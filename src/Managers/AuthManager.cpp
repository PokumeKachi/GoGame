#include "../Managers/AuthManager.h"
#include "../Network/NetworkClient.h"
#include "../Network/Protocol.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

AuthManager::AuthManager(QObject *parent) : QObject(parent) { initDatabase(); }

void AuthManager::setNetworkClient(NetworkClient *client) {
  m_networkClient = client;
}

void AuthManager::initDatabase() {
  const QString appDataDir =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir dir(appDataDir);
  if (!dir.exists()) {
    dir.mkpath(appDataDir);
  }

  const QString dbPath = dir.filePath(QStringLiteral("users.sqlite"));
  QSqlDatabase db;

  if (QSqlDatabase::contains(QStringLiteral("AuthConnection"))) {
    db = QSqlDatabase::database(QStringLiteral("AuthConnection"));
  } else {
    db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"),
                                   QStringLiteral("AuthConnection"));
    db.setDatabaseName(dbPath);
  }

  if (!db.open()) {
    qWarning() << "Failed to open Auth SQLite database:"
               << db.lastError().text();
    setStatusMessage(QStringLiteral("Database connection error"));
    return;
  }

  QSqlQuery query(db);
  const QString createTableSql =
      QStringLiteral("CREATE TABLE IF NOT EXISTS users ("
                     "  username TEXT PRIMARY KEY,"
                     "  password_hash TEXT NOT NULL,"
                     "  created_at TEXT NOT NULL,"
                     "  decks BLOB NOT NULL"
                     ")");

  if (!query.exec(createTableSql)) {
    qWarning() << "Failed to create users table:" << query.lastError().text();
  } else {
    qDebug() << "Auth Database initialized successfully at:" << dbPath;
  }
}

QString AuthManager::hashPassword(const QString &password) const {
  const QByteArray hashed =
      QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
  return QString::fromUtf8(hashed.toHex());
}

void AuthManager::setStatusMessage(const QString &msg) {
  if (m_statusMessage == msg)
    return;
  m_statusMessage = msg;
  emit statusMessageChanged();
}

bool AuthManager::login(const QString &username, const QString &password) {
  const QString cleanUser = username.trimmed();
  if (cleanUser.isEmpty()) {
    const QString err = QStringLiteral("Username cannot be empty");
    setStatusMessage(err);
    emit loginFailed(err);
    return false;
  }

  if (password.isEmpty()) {
    const QString err = QStringLiteral("Password cannot be empty");
    setStatusMessage(err);
    emit loginFailed(err);
    return false;
  }

  QSqlDatabase db = QSqlDatabase::database(QStringLiteral("AuthConnection"));
  if (!db.isOpen() && !db.open()) {
    const QString err = QStringLiteral("Database connection failed");
    setStatusMessage(err);
    emit loginFailed(err);
    return false;
  }

  QSqlQuery query(db);
  query.prepare(
      QStringLiteral("SELECT password_hash FROM users WHERE username = :user"));
  query.bindValue(QStringLiteral(":user"), cleanUser);

  if (!query.exec() || !query.next()) {
    const QString err = QStringLiteral("Invalid username or password");
    setStatusMessage(err);
    emit loginFailed(err);
    return false;
  }

  const QString storedHash = query.value(0).toString();
  const QString inputHash = hashPassword(password);

  if (storedHash != inputHash) {
    const QString err = QStringLiteral("Invalid username or password");
    setStatusMessage(err);
    emit loginFailed(err);
    return false;
  }

  m_username = cleanUser;
  m_isAuthenticated = true;
  setStatusMessage(QStringLiteral("Login successful"));

  emit usernameChanged();
  emit authStateChanged();
  emit loginSuccess(m_username);

  if (m_networkClient && m_networkClient->isConnected()) {
    m_networkClient->sendMessage(Protocol::makeAuthRequest(m_username));
  }

  return true;
}

bool AuthManager::registerUser(const QString &username,
                               const QString &password) {
  const QString cleanUser = username.trimmed();
  if (cleanUser.length() < 3) {
    const QString err =
        QStringLiteral("Username must be at least 3 characters");
    setStatusMessage(err);
    emit registerFailed(err);
    return false;
  }

  if (password.length() < 4) {
    const QString err =
        QStringLiteral("Password must be at least 4 characters");
    setStatusMessage(err);
    emit registerFailed(err);
    return false;
  }

  QSqlDatabase db = QSqlDatabase::database(QStringLiteral("AuthConnection"));
  if (!db.isOpen() && !db.open()) {
    const QString err = QStringLiteral("Database connection failed");
    setStatusMessage(err);
    emit registerFailed(err);
    return false;
  }

  QSqlQuery checkQuery(db);
  checkQuery.prepare(
      QStringLiteral("SELECT username FROM users WHERE username = :user"));
  checkQuery.bindValue(QStringLiteral(":user"), cleanUser);
  if (checkQuery.exec() && checkQuery.next()) {
    const QString err = QStringLiteral("Username already exists");
    setStatusMessage(err);
    emit registerFailed(err);
    return false;
  }

  QSqlQuery insertQuery(db);
  insertQuery.prepare(QStringLiteral(
      "INSERT INTO users (username, password_hash, created_at, decks) "
      "VALUES (:user, :hash, :time, :decks)"));
  insertQuery.bindValue(QStringLiteral(":user"), cleanUser);
  insertQuery.bindValue(QStringLiteral(":hash"), hashPassword(password));
  insertQuery.bindValue(QStringLiteral(":time"),
                        QDateTime::currentDateTime().toString(Qt::ISODate));

  QJsonObject blankDeckProfile;
  QJsonObject deckObj;
  deckObj["chars"] = QJsonArray();
  deckObj["cards"] = QJsonArray();
  blankDeckProfile["deck1"] = deckObj;
  blankDeckProfile["deck2"] = deckObj;
  blankDeckProfile["deck3"] = deckObj;
  QJsonDocument doc(blankDeckProfile);
  QString jsonString = doc.toJson(QJsonDocument::Compact);
  insertQuery.bindValue(QStringLiteral(":decks"), jsonString);

  if (!insertQuery.exec()) {
    const QString err = QStringLiteral("Failed to create account: ") +
                        insertQuery.lastError().text();
    setStatusMessage(err);
    emit registerFailed(err);
    return false;
  }

  setStatusMessage(QStringLiteral("Account created! Logging in..."));
  emit registerSuccess(cleanUser);

  // Auto login
  return login(cleanUser, password);
}

void AuthManager::logout() {
  qDebug() << "Logged out" << '\n';
  m_username.clear();
  m_isAuthenticated = false;
  setStatusMessage(QStringLiteral("Logged out"));
  emit usernameChanged();
  emit authStateChanged();
}

void AuthManager::handleServerMessage(const QJsonObject &message) {
  const QString type = Protocol::readMessageType(message);
  const QJsonObject payload = Protocol::readPayload(message);

  if (type == "AuthResult") {
    const bool success = payload.value("success").toBool(true);
    if (!success) {
      setStatusMessage(
          payload.value("message").toString("Server authentication rejected"));
    }
  }
}
