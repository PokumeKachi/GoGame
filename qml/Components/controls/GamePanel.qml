import QtQuick
import lumieTcg

Rectangle {
    id: root

    property color backgroundColor: Qt.rgba(0.10, 0.11, 0.14, 0.82)
    property color borderColor: Qt.rgba(Theme.primary.r, Theme.primary.g, Theme.primary.b, 0.28)
    property int radiusSize: Radius.lg
    property bool highlighted: false

    color: backgroundColor
    radius: radiusSize
    border.width: highlighted ? 2 : 1
    border.color: highlighted ? Theme.primary : borderColor
    layer.enabled: true

    Behavior on border.color {
        ColorAnimation { duration: Motion.fast }
    }

    Behavior on scale {
        NumberAnimation { duration: Motion.fast; easing.type: Easing.OutCubic }
    }
}
