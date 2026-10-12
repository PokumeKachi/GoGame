import QtQuick
import lumieTcg

Rectangle {
    id: root
    property alias text: label.text
    property color backgroundColor: Theme.panelBackground
    property color accentColor: Theme.primary
    signal clicked

    implicitWidth: 160
    implicitHeight: 46
    radius: 7
    opacity: enabled ? 1 : 0.5
    color: mouse.pressed ? Theme.primary : mouse.containsMouse ? Theme.secondary : backgroundColor
    border.width: 2
    border.color: accentColor
    scale: mouse.pressed ? 0.97 : 1
    Behavior on scale {
        NumberAnimation {
            duration: Motion.fast
        }
    }
    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: root.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        onClicked: root.clicked()
    }
    Text {
        id: label
        anchors.fill: parent
        anchors.margins: 8
        color: Theme.text
        font.pixelSize: 16
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
