// src/Managers/MatchmakingManager.cpp
#include "MatchmakingManager.h"

#include "../Network/Protocol.h"

MatchmakingManager::MatchmakingManager(QObject* parent)
    : QObject(parent)
{
}

void MatchmakingManager::setNetworkClient(NetworkClient* networkClient)
{
    m_networkClient = networkClient;
}

bool MatchmakingManager::isSearching() const
{
    return m_searching;
}

QString MatchmakingManager::statusText() const
{
    return m_statusText;
}

void MatchmakingManager::startMatchmaking(const QString& mode, const QString& deckId)
{
    if (!m_networkClient) {
        emit matchmakingError(QStringLiteral("Network client is not ready"));
        return;
    }

    if (!m_networkClient->isConnected()) {
        emit matchmakingError(QStringLiteral("Not connected to server"));
        return;
    }

    setSearching(true);
    setStatusText(QStringLiteral("Finding opponent..."));

    m_networkClient->sendMessage(
        Protocol::makeStartMatchmaking(mode, deckId)
        );
}

void MatchmakingManager::cancelMatchmaking()
{
    if (!m_networkClient) {
        return;
    }

    m_networkClient->sendMessage(
        Protocol::makeCancelMatchmaking()
        );

    setSearching(false);
    setStatusText(QStringLiteral("Matchmaking cancelled"));
}

void MatchmakingManager::handleServerMessage(const QJsonObject& message)
{
    const QString type = Protocol::readMessageType(message);
    const QJsonObject payload = Protocol::readPayload(message);

    if (type == "MatchmakingStatus") {
        const QString status = payload.value("status").toString();
        setStatusText(status);
        return;
    }

    if (type == "MatchFound") {
        setSearching(false);
        setStatusText(QStringLiteral("Match found"));

        const QString matchId = payload.value("matchId").toString();
        const int playerIndex = payload.value("playerIndex").toInt();
        const QString opponentName = payload.value("opponentName").toString();

        emit matchFound(matchId, playerIndex, opponentName);
        return;
    }

    if (type == "Error") {
        const QString errorMessage = payload.value("message").toString();
        emit matchmakingError(errorMessage);
    }
}

void MatchmakingManager::setSearching(bool searching)
{
    if (m_searching == searching) {
        return;
    }

    m_searching = searching;
    emit searchingChanged();
}

void MatchmakingManager::setStatusText(const QString& statusText)
{
    if (m_statusText == statusText) {
        return;
    }

    m_statusText = statusText;
    emit statusTextChanged();
}