#include <QCoreApplication>
#include "../../src/Network/NetworkClient.h"
#include <QEventLoop>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QTemporaryDir>
#include <QTimer>
#include <QWebSocket>
#include <cstdlib>
#include <cstdio>

void check(bool ok, const char *message)
{
    if (!ok) {
        std::fprintf(stderr, "%s\n", message);
        std::exit(1);
    }
}

QJsonObject exchange(QWebSocket &socket, const QString &message)
{
    QEventLoop loop;
    QString response;
    auto connection = QObject::connect(&socket, &QWebSocket::textMessageReceived,
                                      &loop, [&](const QString &text) {
        response = text;
        loop.quit();
    });
    QTimer::singleShot(3000, &loop, &QEventLoop::quit);
    socket.sendTextMessage(message);
    loop.exec();
    QObject::disconnect(connection);
    check(!response.isEmpty(), "Response timeout");
    return QJsonDocument::fromJson(response.toUtf8()).object();
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    check(argc == 2, "Expected server executable path");
    QTemporaryDir storage;
    check(storage.isValid(), "Temporary directory failed");
    const QString path = storage.filePath("state.json");
    QProcess server;
    auto start = [&] {
        server.start(QString::fromLocal8Bit(argv[1]), {"--port", "24509", "--data-file", path});
        check(server.waitForStarted(), "Server failed to start");
        if (!server.waitForReadyRead(3000)) {
            std::fprintf(stderr, "%s", server.readAllStandardError().constData());
            check(false, "Server did not report readiness");
        }
        check(server.state() == QProcess::Running, "Server exited unexpectedly");
    };
    auto stop = [&] {
        server.terminate();
        check(server.waitForFinished(), "Server failed to stop");
    };
    auto connectSocket = [&](QWebSocket &socket) {
        QEventLoop loop;
        QObject::connect(&socket, &QWebSocket::connected, &loop, &QEventLoop::quit);
        QTimer::singleShot(3000, &loop, &QEventLoop::quit);
        socket.open(QUrl("ws://127.0.0.1:24509"));
        loop.exec();
        check(socket.state() == QAbstractSocket::ConnectedState, "Connection failed");
    };
    start();
    qputenv("GOGAME_SERVER_URL", "ws://127.0.0.1:24509");
    NetworkClient client;
    auto connectClient = [&] {
        QEventLoop loop;
        QObject::connect(&client, &NetworkClient::connected, &loop, &QEventLoop::quit);
        QObject::connect(&client, &NetworkClient::errorOccurred, &loop, &QEventLoop::quit);
        QTimer::singleShot(3000, &loop, &QEventLoop::quit);
        client.connectToServer(client.serverUrl());
        loop.exec();
        check(client.isConnected(), "NetworkClient failed to connect");
        check(client.lastError().isEmpty(), "Connection retained an old error");
    };
    connectClient();
    {
        QEventLoop loop;
        QJsonObject response;
        QObject::connect(&client, &NetworkClient::messageReceived, &loop,
                         [&](const QJsonObject &message) { response = message; loop.quit(); });
        QTimer::singleShot(3000, &loop, &QEventLoop::quit);
        client.sendMessage({{"type", "ReadData"}});
        loop.exec();
        check(response["type"] == "Data", "NetworkClient failed to receive server JSON");
    }
    client.connectToServer("invalid-url");
    check(!client.lastError().isEmpty(), "Invalid URL did not report an error");
    client.disconnectFromServer();
    connectClient();
    QWebSocket first;
    connectSocket(first);
    auto response = exchange(first, R"({"type":"ReadData"})");
    check(response["type"] == "Data" && response["payload"].toObject()["data"].toObject().isEmpty(),
          "Initial file must contain an empty object");
    response = exchange(first, R"({"type":"WriteData","payload":{"data":{"turn":1,"board":[0,-1,1]}}})");
    check(response["type"] == "DataWritten", "Write failed");
    QFile file(path);
    check(file.open(QIODevice::ReadOnly), "Cannot inspect JSON file");
    check(QJsonDocument::fromJson(file.readAll()).object()["turn"].toInt() == 1,
          "Data was not persisted");
    file.close();
    check(exchange(first, "invalid")["type"] == "Error", "Malformed JSON accepted");
    check(exchange(first, "[]")["type"] == "Error", "JSON array accepted");
    check(exchange(first, R"({"type":"WriteData","payload":{"data":[]}})")["type"] == "Error",
          "Invalid payload accepted");
    check(exchange(first, R"({"type":"Unknown"})")["type"] == "Error", "Unknown type accepted");
    QWebSocket second;
    connectSocket(second);
    response = exchange(second, R"({"type":"ReadData"})");
    check(response["payload"].toObject()["data"].toObject()["turn"].toInt() == 1,
          "Second client cannot read saved data");
    stop();
    start();
    QWebSocket afterRestart;
    connectSocket(afterRestart);
    response = exchange(afterRestart, R"({"type":"ReadData"})");
    check(response["payload"].toObject()["data"].toObject()["turn"].toInt() == 1,
          "Data lost after restart");
    stop();
    check(file.open(QIODevice::WriteOnly | QIODevice::Truncate), "Cannot prepare corrupt file");
    file.write("broken json");
    file.close();
    server.start(QString::fromLocal8Bit(argv[1]), {"--data-file", path});
    check(server.waitForFinished(3000) && server.exitCode() != 0,
          "Server accepted a corrupt data file");
    check(file.open(QIODevice::ReadOnly) && file.readAll() == "broken json",
          "Corrupt file was overwritten");
}
