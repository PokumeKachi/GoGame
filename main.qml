import QtQuick
import QtQuick.Controls
import "qml/Screens"
import "qml/Core"

ApplicationWindow {
    id: root

    visible: true

    width: 1440
    height: 900

    minimumWidth: 1280
    minimumHeight: 720

    maximumWidth: 1280
    maximumHeight: 720

    title: "Go Game"

    SceneManager {
        anchors.fill: parent
    }
}
