import QtQuick
import QtQuick.Layouts
import "../controls"
import lumieTcg

GamePanel {
    id: root

    property int current: 10
    property int maximum: 10
    property string label: "Element Points"

    implicitWidth: 360
    implicitHeight: 72
    radiusSize: Radius.md

    RowLayout {
        anchors.fill: parent
        anchors.margins: Spacing.md
        spacing: Spacing.sm

        GameText {
            text: root.label
            type: GameText.Caption
            color: Theme.textGray
            Layout.fillWidth: true
        }

        Repeater {
            model: root.maximum

            Rectangle {
                required property int index

                Layout.preferredWidth: 18
                Layout.preferredHeight: 30
                radius: Radius.pill
                color: index < root.current ? Theme.primary : Qt.rgba(1, 1, 1, 0.10)
                border.width: 1
                border.color: index < root.current ? "#F8E3A6" : Qt.rgba(1, 1, 1, 0.18)

                Behavior on color {
                    ColorAnimation { duration: Motion.medium }
                }
            }
        }

        GameText {
            text: root.current + "/" + root.maximum
            type: GameText.Caption
            color: Theme.primary
            horizontalAlignment: Text.AlignRight
            Layout.preferredWidth: 54
        }
    }
}
