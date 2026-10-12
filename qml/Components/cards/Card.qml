import QtQuick
import "../controls"

GamePanel {
    id: root
    property string cardId: ""
    property string title: cardId
    property int cost: 0
    property bool selected: false
    signal activated()
    width: 112; height: 158
    highlighted: selected
    Image { anchors.fill: parent; anchors.margins: 5; source: assetsManager.resolveCardImage(root.cardId); fillMode: Image.PreserveAspectFit }
    Rectangle { anchors.left: parent.left; anchors.right: parent.right; anchors.bottom: parent.bottom; height: 31; color: "#c0000000"; radius: 8 }
    Text { anchors.left: parent.left; anchors.leftMargin: 7; anchors.right: costText.left; anchors.bottom: parent.bottom; anchors.bottomMargin: 8; text: root.title; color: "white"; font.pixelSize: 10; elide: Text.ElideRight }
    Text { id: costText; anchors.right: parent.right; anchors.rightMargin: 7; anchors.bottom: parent.bottom; anchors.bottomMargin: 7; text: root.cost; color: "#8ed8ff"; font.bold: true }
    MouseArea { anchors.fill: parent; onClicked: root.activated() }
}
