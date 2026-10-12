#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSaveFile>
#include <QWebSocket>
#include <QWebSocketServer>
#include <cstdio>

namespace {
bool writeData(const QString &path, const QJsonObject &data, QString &error)
{
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        error = file.errorString();
        return false;
    }
    const QByteArray bytes = QJsonDocument(data).toJson(QJsonDocument::Indented);
    if (file.write(bytes) != bytes.size() || !file.commit()) {
        error = file.errorString();
        return false;
    }
    return true;
}

bool readData(const QString &path, QJsonObject &data, QString &error)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        error = file.errorString();
        return false;
    }
    const QByteArray bytes = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        error = file.errorString();
        return false;
    }
    QJsonParseError parseError;
    const auto document = QJsonDocument::fromJson(bytes, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        error = QStringLiteral("Data file must contain a valid JSON object");
        return false;
    }
    data = document.object();
    return true;
}

void reply(QWebSocket *socket, const QString &type, const QJsonObject &payload)
{
    socket->sendTextMessage(QString::fromUtf8(
        QJsonDocument(QJsonObject{{"type", type}, {"payload", payload}})
            .toJson(QJsonDocument::Compact)));
}

void fail(QWebSocket *socket, const QString &code, const QString &message)
{
    reply(socket, QStringLiteral("Error"), {{"code", code}, {"message", message}});
}
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("GoGameServer"));
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Local WebSocket JSON file server"));
    parser.addHelpOption();
    parser.addOption({{"p", "port"}, "Listen port (localhost only).", "port", "14509"});
    parser.addOption({{"d", "data-file"}, "JSON storage file.", "path", "data/state.json"});
    parser.process(app);

    bool portOk = false;
    const uint port = parser.value("port").toUInt(&portOk);
    if (!portOk || port == 0 || port > 65535) {
        std::fprintf(stderr, "Port must be between 1 and 65535\n");
        return 1;
    }
    const QString path = QFileInfo(parser.value("data-file")).absoluteFilePath();
    if (!QDir().mkpath(QFileInfo(path).absolutePath())) {
        std::fprintf(stderr, "Cannot create storage directory\n");
        return 1;
    }
    QString error;
    QJsonObject initialData;
    if ((!QFileInfo::exists(path) && !writeData(path, {}, error))
        || !readData(path, initialData, error)) {
        std::fprintf(stderr, "%s\n", qPrintable(error));
        return 1;
    }

    QWebSocketServer server(QStringLiteral("GoGameServer"), QWebSocketServer::NonSecureMode);
    QObject::connect(&server, &QWebSocketServer::newConnection, &app, [&] {
        QWebSocket *socket = server.nextPendingConnection();
        if (!socket)
            return;
        socket->setParent(&server);
        socket->setMaxAllowedIncomingMessageSize(1024 * 1024);
        QObject::connect(socket, &QWebSocket::disconnected, socket, &QObject::deleteLater);
        QObject::connect(socket, &QWebSocket::textMessageReceived, socket,
                         [socket, path](const QString &text) {
                             QJsonParseError parseError;
                             const auto document = QJsonDocument::fromJson(text.toUtf8(), &parseError);
                             if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
                                 fail(socket, "InvalidJson", "Message must be a JSON object");
                                 return;
                             }
                             const auto request = document.object();
                             const QString type = request.value("type").toString();
                             QString error;
                             if (type == "ReadData") {
                                 QJsonObject data;
                                 if (!readData(path, data, error)) {
                                     fail(socket, "ReadFailed", error);
                                     return;
                                 }
                                 reply(socket, "Data", {{"data", data}});
                             } else if (type == "WriteData") {
                                 const auto payload = request.value("payload");
                                 if (!payload.isObject() || !payload.toObject().value("data").isObject()) {
                                     fail(socket, "InvalidPayload", "payload.data must be a JSON object");
                                     return;
                                 }
                                 if (!writeData(path, payload.toObject().value("data").toObject(), error)) {
                                     fail(socket, "WriteFailed", error);
                                     return;
                                 }
                                 reply(socket, "DataWritten", {{"ok", true}});
                             } else {
                                 fail(socket, "UnknownType", "Supported types: ReadData, WriteData");
                             }
                         });
        QObject::connect(socket, &QWebSocket::binaryMessageReceived, socket,
                         [socket](const QByteArray &) {
                             fail(socket, "UnsupportedMessage", "Send JSON as a text message");
                         });
    });
    if (!server.listen(QHostAddress::LocalHost, static_cast<quint16>(port))) {
        std::fprintf(stderr, "%s\n", qPrintable(server.errorString()));
        return 1;
    }
    std::fprintf(stdout, "Listening on ws://127.0.0.1:%u\nJSON storage: %s\n",
                 port, qPrintable(path));
    std::fflush(stdout);
    return app.exec();
}
