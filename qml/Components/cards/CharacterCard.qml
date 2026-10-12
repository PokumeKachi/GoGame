import QtQuick
import "../controls"

GamePanel {
    id: root
    property string characterId: ""
    property int hp: 10
    property int energy: 0
    property bool active: false
    signal activated()
    width: 142; height: 170
    highlighted: active
    Image { anchors.fill: parent; anchors.margins: 5; source: assetsManager.resolveCharacterImage(root.characterId); fillMode: Image.PreserveAspectCrop }
    Rectangle { anchors.left: parent.left; anchors.right: parent.right; anchors.bottom: parent.bottom; height: 31; color: "#c0000000" }
    Text { anchors.left: parent.left; anchors.leftMargin: 8; anchors.bottom: parent.bottom; anchors.bottomMargin: 8; text: root.characterId; color: "white"; font.pixelSize: 11 }
    Text { anchors.right: parent.right; anchors.rightMargin: 8; anchors.bottom: parent.bottom; anchors.bottomMargin: 7; text: "♥ " + root.hp; color: "#ff8b8b"; font.bold: true }
    MouseArea { anchors.fill: parent; onClicked: root.activated() }
}
