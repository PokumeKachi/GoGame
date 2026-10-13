// src/Managers/AssetManager.cpp
#include "AssetManager.h"

#include <QAudioOutput>
#include <QFont>
#include <QFontDatabase>
#include <QMediaPlayer>
#include <QObject>
#include <QPixmap>


AssetManager::AssetManager(QObject *parent)
    : QObject(parent) {}

const QHash<QString, QString> AssetManager::paths = {
    {"GenshinFont", "qrc:/assets/fonts/genshin.ttf"},
    // {"Background", "qrc:/assets/background"}
};

QUrl AssetManager::url(const QString &name) const {
    return QUrl(paths.value(name));
}
