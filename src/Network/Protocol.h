// src/Network/Protocol.h
#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QVariantMap>

#include "../Core/Enums.h"

namespace Protocol {

QJsonObject makeEnvelope(NetworkMessageType type, const QJsonObject &payload);
QJsonObject makeErrorPayload(const QString &code, const QString &message);

QJsonObject makeAuthRequest(const QString &nickname);
QJsonObject makeStartMatchmaking(const QString &mode, const QString &deckId);
QJsonObject makeCancelMatchmaking();

QJsonObject makeSubmitDeck(const QString &deckId,
                           const QStringList &characterIds,
                           const QStringList &cardIds);

QJsonObject makeGameCommand(const QString &matchId, GameCommandType commandType,
                            const QJsonObject &commandPayload);

QJsonObject makeChooseActiveCharacterCommand(const QString &matchId,
                                             int characterIndex);

QJsonObject makeEndRoundCommand(const QString &matchId);
QJsonObject makeSwitchCharacterCommand(const QString &matchId,
                                       int characterIndex);

QJsonObject makeUseSkillCommand(const QString &matchId, int skillIndex,
                                int elementPointCost,
                                const QVariantMap &target);

QJsonObject makePlayCardCommand(const QString &matchId, int handIndex,
                                int elementPointCost,
                                const QVariantMap &target);

QString readMessageType(const QJsonObject &message);
QJsonObject readPayload(const QJsonObject &message);

} // namespace Protocol
