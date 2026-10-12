// src/Core/EnumUtils.h
#pragma once

#include <QString>

#include "Enums.h"

namespace EnumUtils {

QString toString(NetworkMessageType type);
NetworkMessageType networkMessageTypeFromString(const QString& value);

QString toString(GameStage stage);
GameStage gameStageFromString(const QString& value);

QString toString(GameCommandType type);
GameCommandType gameCommandTypeFromString(const QString& value);

QString toString(GameEventType type);
GameEventType gameEventTypeFromString(const QString& value);
}