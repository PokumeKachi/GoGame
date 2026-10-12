import QtQuick

Item {
    id: root
    property string text: "Loading"
    width: 150; height: 70
    Text { anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top; text: "✦"; color: "#e4c477"; font.pixelSize: 32; RotationAnimation on rotation { from: 0; to: 360; duration: 1200; loops: Animation.Infinite } }
    Text { anchors.horizontalCenter: parent.horizontalCenter; anchors.bottom: parent.bottom; text: root.text; color: "white"; font.pixelSize: 14 }
}
