import QtQuick
import QtQuick.Controls

Popup {
    id: root
    property string title: ""
    property string message: ""
    modal: true; focus: true
    anchors.centerIn: Overlay.overlay
    width: 420; height: 210
    background: Rectangle { radius: 16; color: "#f21c2736"; border.width: 2; border.color: "#d7b26d" }
    contentItem: Column {
        spacing: 18
        Text { text: root.title; color: "#f2d38b"; font.pixelSize: 22; font.bold: true }
        Text { text: root.message; color: "white"; width: 370; wrapMode: Text.WordWrap; font.pixelSize: 14 }
        Button { text: "Confirm"; anchors.horizontalCenter: parent.horizontalCenter; onClicked: root.close() }
    }
}
