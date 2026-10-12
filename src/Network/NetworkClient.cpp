// src/Network/NetworkClient.cpp
#include "NetworkClient.h"

#include <QJsonDocument>
#include <QJsonParseError>

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent),
    m_serverUrl(qEnvironmentVariable(
                    "GOGAME_SERVER_URL",
                    QStringLiteral("ws://127.0.0.1:14509"))
                    .trimmed()) {
    connect(&m_socket, &QWebSocket::connected, this,
            &NetworkClient::handleConnected);

    connect(&m_socket, &QWebSocket::disconnected, this,
            &NetworkClient::handleDisconnected);

    connect(&m_socket, &QWebSocket::textMessageReceived, this,
            &NetworkClient::handleTextMessageReceived);

    connect(&m_socket, &QWebSocket::errorOccurred, this,
            &NetworkClient::handleErrorOccurred);
}

bool NetworkClient::isConnected() const { return m_connected; }

QString NetworkClient::lastError() const { return m_lastError; }

void NetworkClient::connectToServer(const QString &url) {
    const QUrl endpoint(url.trimmed(), QUrl::StrictMode);
    if (!endpoint.isValid() || endpoint.host().isEmpty() ||
        (endpoint.scheme() != QStringLiteral("ws") &&
         endpoint.scheme() != QStringLiteral("wss"))) {
        setLastError(QStringLiteral("Invalid WebSocket URL. Use wss://hostname or "
                                    "ws://host:port for local development."));
        emit errorOccurred(m_lastError);
        return;
    }
    m_socket.abort();
    setLastError(QString());
    m_socket.open(endpoint);
}

QString NetworkClient::serverUrl() const { return m_serverUrl; }

void NetworkClient::disconnectFromServer() { m_socket.close(); }

void NetworkClient::sendMessage(const QJsonObject &message) {
    if (!m_connected) {
        setLastError(QStringLiteral("Not connected to server"));
        emit errorOccurred(m_lastError);
        return;
    }

    const QJsonDocument document(message);
    const QString rawMessage =
        QString::fromUtf8(document.toJson(QJsonDocument::Compact));

    m_socket.sendTextMessage(rawMessage);
}

void NetworkClient::handleConnected() {
    setLastError(QString());
    m_connected = true;
    emit connectedChanged();
    emit connected();
}

void NetworkClient::handleDisconnected() {
    m_connected = false;
    emit connectedChanged();
    emit disconnected();
}

void NetworkClient::handleTextMessageReceived(const QString &message) {
    QJsonParseError parseError;
    const QJsonDocument document =
        QJsonDocument::fromJson(message.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        setLastError(QStringLiteral("Invalid JSON from server: %1")
                         .arg(parseError.errorString()));
        emit errorOccurred(m_lastError);
        return;
    }

    if (!document.isObject()) {
        setLastError(QStringLiteral("Server message is not a JSON object"));
        emit errorOccurred(m_lastError);
        return;
    }

    emit messageReceived(document.object());
}

void NetworkClient::handleErrorOccurred(QAbstractSocket::SocketError) {
    setLastError(m_socket.errorString());
    emit errorOccurred(m_lastError);
}

void NetworkClient::setLastError(const QString &error) {
    if (m_lastError == error) {
        return;
    }

    m_lastError = error;
    emit lastErrorChanged();
}
