pragma Singleton

import QtQuick

QtObject {
    readonly property FontLoader genshinFont: FontLoader {
        id: genshin
        source: "/lumieTcg/assets/fonts/genshin.ttf"
    }

    readonly property string family: genshin.name

    readonly property int display: 64
    readonly property int title: 42
    readonly property int heading: 30
    readonly property int body: 22
    readonly property int caption: 18

}