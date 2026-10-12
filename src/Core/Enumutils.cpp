#pragma once

#include <QString>

#include "Enumutils.h"


namespace {

QString normalize(const QString& value)
{
    return value.trimmed().toLower();
}

}

namespace EnumUtils{
QString toString(GameStage stage)
{
    switch (stage) {
    case GameStage::None: return QStringLiteral("None");
    case GameStage::GameStart: return QStringLiteral("GameStart");
    case GameStage::RoundStart: return QStringLiteral("RoundStart");
    case GameStage::Roll: return QStringLiteral("Roll");
    case GameStage::Action: return QStringLiteral("Action");
    case GameStage::RoundEnd: return QStringLiteral("RoundEnd");
    case GameStage::GameEnd: return QStringLiteral("GameEnd");
    }

    return QStringLiteral("None");
}

GameStage gameStageFromString(const QString& value)
{
    const QString v = normalize(value);

    if (v == "gamestart") return GameStage::GameStart;
    if (v == "roundstart") return GameStage::RoundStart;
    if (v == "roll") return GameStage::Roll;
    if (v == "action") return GameStage::Action;
    if (v == "roundend") return GameStage::RoundEnd;
    if (v == "gameend") return GameStage::GameEnd;

    return GameStage::None;
}
QString toString(NetworkMessageType type)
{
    switch (type) {
    case NetworkMessageType::Unknown: return QStringLiteral("Unknown");
    case NetworkMessageType::AuthRequest: return QStringLiteral("AuthRequest");
    case NetworkMessageType::AuthResult: return QStringLiteral("AuthResult");
    case NetworkMessageType::MatchmakingStart: return QStringLiteral("MatchmakingStart");
    case NetworkMessageType::MatchmakingCancel: return QStringLiteral("MatchmakingCancel");
    case NetworkMessageType::MatchFound: return QStringLiteral("MatchFound");
    case NetworkMessageType::MatchmakingStatus: return QStringLiteral("MatchmakingStatus");
    case NetworkMessageType::SubmitDeck: return QStringLiteral("SubmitDeck");
    case NetworkMessageType::GameSnapshot: return QStringLiteral("GameSnapshot");
    case NetworkMessageType::GameCommand: return QStringLiteral("GameCommand");
    case NetworkMessageType::GameEvent: return QStringLiteral("GameEvent");
    case NetworkMessageType::Error: return QStringLiteral("Error");
    }

    return QStringLiteral("Unknown");
}

NetworkMessageType networkMessageTypeFromString(const QString& value)
{
    const QString v = normalize(value);

    if (v == "authrequest") return NetworkMessageType::AuthRequest;
    if (v == "authresult") return NetworkMessageType::AuthResult;
    if (v == "matchmakingstart") return NetworkMessageType::MatchmakingStart;
    if (v == "matchmakingcancel") return NetworkMessageType::MatchmakingCancel;
    if (v == "matchfound") return NetworkMessageType::MatchFound;
    if (v == "matchmakingstatus") return NetworkMessageType::MatchmakingStatus;
    if (v == "submitdeck") return NetworkMessageType::SubmitDeck;
    if (v == "gamesnapshot") return NetworkMessageType::GameSnapshot;
    if (v == "gamecommand") return NetworkMessageType::GameCommand;
    if (v == "gameevent") return NetworkMessageType::GameEvent;
    if (v == "error") return NetworkMessageType::Error;

    return NetworkMessageType::Unknown;
}

QString toString(GameCommandType type)
{
    switch (type) {
    case GameCommandType::Unknown: return QStringLiteral("Unknown");
    case GameCommandType::Play: return QStringLiteral("Play");
    case GameCommandType::EndRound: return QStringLiteral("EndRound");
    case GameCommandType::Concede: return QStringLiteral("Concede");
    }

    return QStringLiteral("Unknown");
}
}