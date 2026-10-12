// src/Managers/MatchmakingManager.h
#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>

#include "../Network/NetworkClient.h"

class MatchmakingManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool searching READ isSearching NOTIFY searchingChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)

public:
    explicit MatchmakingManager(QObject* parent = nullptr);

    void setNetworkClient(NetworkClient* networkClient);

    bool isSearching() const;
    QString statusText() const;

    Q_INVOKABLE void startMatchmaking(const QString& mode, const QString& deckId);
    Q_INVOKABLE void cancelMatchmaking();

signals:
    void searchingChanged();
    void statusTextChanged();

    void matchFound(const QString& matchId, int playerIndex, const QString& opponentName);
    void matchmakingError(const QString& message);

public slots:
    void handleServerMessage(const QJsonObject& message);

private:
    void setSearching(bool searching);
    void setStatusText(const QString& statusText);

private:
    NetworkClient* m_networkClient = nullptr;
    bool m_searching = false;
    QString m_statusText;
};