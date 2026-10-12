#include "Game.h"

#include "../Core/Enumutils.h"
#include "../Network/Protocol.h"

#include <QJsonArray>

Game::Game(QObject* parent) : QObject(parent)
{ // init do it later
}

void Game::setNetworkClient(NetworkClient* networkClient)
{
    m_networkClient = networkClient;
}

QString Game::matchId() const
{
    return m_matchId;
}

int Game::playerIndex() const
{
    return m_playerIndex;
}

QString Game::opponentName() const
{
    return m_opponentName;
}

void Game::setMatchInfo(const QString& matchId, int playerIndex, const QString& opponentName) {

}

void Game::handleServerMessage(const QJsonObject& message) {

}