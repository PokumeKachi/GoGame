// src/Managers/AuthManager.h
#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>

class NetworkClient;

class AuthManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString username READ username NOTIFY usernameChanged)
    Q_PROPERTY(QString nickname READ username NOTIFY usernameChanged)
    Q_PROPERTY(bool isAuthenticated READ isAuthenticated NOTIFY authStateChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit AuthManager(QObject* parent = nullptr);
    ~AuthManager() override = default;

    void setNetworkClient(NetworkClient* client);

    QString username() const { return m_username; }
    bool isAuthenticated() const { return m_isAuthenticated; }
    QString statusMessage() const { return m_statusMessage; }

    Q_INVOKABLE bool login(const QString& username, const QString& password);
    Q_INVOKABLE bool registerUser(const QString& username, const QString& password);
    Q_INVOKABLE void logout();

signals:
    void usernameChanged();
    void authStateChanged();
    void statusMessageChanged();
    void loginSuccess(const QString& username);
    void loginFailed(const QString& error);
    void registerSuccess(const QString& username);
    void registerFailed(const QString& error);

public slots:
    void handleServerMessage(const QJsonObject& message);

private:
    void initDatabase();
    QString hashPassword(const QString& password) const;
    void setStatusMessage(const QString& msg);

private:
    NetworkClient* m_networkClient = nullptr;
    QString m_username;
    bool m_isAuthenticated = false;
    QString m_statusMessage;
};

