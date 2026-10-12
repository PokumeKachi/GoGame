// src/Core/Game.h
#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>

#include "../Core/Enums.h"
#include "../Network/NetworkClient.h"

class Game;

class Game : public QObject {
    Q_OBJECT

public:
    explicit Game(QObject* parent = nullptr);
    void setNetworkClient(NetworkClient* networkClient);
    Q_INVOKABLE void setMatchInfo(
        const QString& matchId,
        int playerIndex,
        const QString& opponentName
        );

    QString matchId() const;
    int playerIndex() const;
    QString opponentName() const;
signals:

public slots:
    void handleServerMessage(const QJsonObject& message);
private:
    NetworkClient* m_networkClient = nullptr;
    QString m_matchId;
    int m_playerIndex = -1;
    QString m_opponentName;
    GameStage m_stage = GameStage::None;
};
