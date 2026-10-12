// src/Core/Enums.h
#pragma once

#include <QString>

enum class NetworkMessageType {
    Unknown,
    AuthRequest,
    AuthResult,
    MatchmakingStart,
    MatchmakingCancel,
    MatchFound,
    MatchmakingStatus,
    SubmitDeck,
    GameSnapshot,
    GameCommand,
    GameEvent,
    Error
};

enum class GameStage {
    None,
    GameStart,
    RoundStart,
    Roll,
    Action,
    RoundEnd,
    GameEnd
};

enum class GameCommandType {
    Unknown,
    Play,
    EndRound,
    Concede
};

enum class GameEventType {
    Unknown,
    GameStarted,
    RoundStarted,
    StageChanged,
    CardsDrawn,
    ElementPointsChanged,
    WeatherChanged,
    CharacterDamaged,
    CharacterHealed,
    EnergyChanged,
    ActiveCharacterChanged,
    CardPlayed,
    SkillUsed,
    SummonCreated,
    SupportCreated,
    StateAttached,
    RoundEnded,
    GameEnded
};