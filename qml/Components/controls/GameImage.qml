import QtQuick

Item {
    id: root
    property alias source: image.source
    property alias fillMode: image.fillMode
    property real cornerRadius: 10
    property color borderColor: "#55d7b26d"
    clip: true
    Rectangle { anchors.fill: parent; radius: root.cornerRadius; color: "#182231"; border.color: root.borderColor }
    Image { id: image; anchors.fill: parent; anchors.margins: 2; fillMode: Image.PreserveAspectFit; asynchronous: true; smooth: true }
}
