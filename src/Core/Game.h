// src/Core/Game.h
#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QVariantList>
#include <vector>

#include "../Core/Enums.h"
#include "../Network/NetworkClient.h"

class Game;

class Game : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList boardState READ boardState NOTIFY boardStateChanged)
    Q_PROPERTY(int currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(int playerIndex READ playerIndex NOTIFY matchInfoChanged)

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
    int currentPlayer() const { return m_currentPlayer; }
    QString opponentName() const;
    QVariantList boardState() const {
        QVariantList list;
        for (int i = 0; i < 19; ++i) {
            for (int j = 0; j < 19; ++j) {
                list.append(m_board_state[i][j]);
            }
        }
        return list;
    }
    std::vector<std::vector<int>> board_state();
    Q_INVOKABLE bool is_valid_move(int x, int y);
    Q_INVOKABLE void update_pieces(int x, int y);
    Q_INVOKABLE void pass_turn();

    void setStage(GameStage stage);
    void sendCommand(const QJsonObject &command);
signals:
    void pieces_updated();
    void boardStateChanged();
    void currentPlayerChanged();
    void matchInfoChanged();
    void snapshotChanged();

    void stageChanged();
    void gameError(const QString& message);
    void gameSnapshotReceived(const QJsonObject& snapshot);
    void gameEventsReceived(const QJsonArray& events);
    void gameEnded(const QString& reason);
public slots:
    void handleServerMessage(const QJsonObject& message);
private:
    NetworkClient* m_networkClient = nullptr;
    QString m_matchId;
    int m_playerIndex = -1;
    int m_currentPlayer = 0;
    QString m_opponentName;
    GameStage m_stage = GameStage::None;
    int m_ko_square = -1;
    std::vector<std::vector<int>> m_board_state;
    QVariantMap m_snapshot;
};
