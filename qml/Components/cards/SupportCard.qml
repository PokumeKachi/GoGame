import QtQuick
import "../controls"

GamePanel {
    id: root
    property string supportId: ""
    property int usage: 0
    width: 96; height: 120
    Image { anchors.fill: parent; anchors.margins: 5; source: assetsManager.resolveCardImage(root.supportId); fillMode: Image.PreserveAspectFit }
    Text { anchors.right: parent.right; anchors.bottom: parent.bottom; anchors.margins: 7; text: root.usage; color: "white"; font.bold: true }
}
