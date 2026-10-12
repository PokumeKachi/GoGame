pragma Singleton

import QtQuick

QtObject {

    readonly property int windowWidth: 1600

    readonly property int windowHeight: 900

    readonly property int sidebarWidth: 340

    readonly property int cardWidth: 180

    readonly property int cardHeight: 310

    readonly property int animationSpeed: 200

    readonly property string serverUrl: networkClient.serverUrl
}
