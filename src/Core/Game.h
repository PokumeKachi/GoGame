// src/Core/Game.h
#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <vector>
#include <iostream>

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
    std::vector<std::vector<int>> board_state();
    Q_INVOKABLE bool is_valid_move(int x, int y);
    Q_INVOKABLE void update_pieces(int x, int y);
signals:
    void pieces_updated();
public slots:
    void handleServerMessage(const QJsonObject& message);
private:
    NetworkClient* m_networkClient = nullptr;
    QString m_matchId;
    int m_playerIndex = -1;
    QString m_opponentName;
    GameStage m_stage = GameStage::None;
    int m_ko_square = -1;
    std::vector<std::vector<int>> m_board_state;
};
