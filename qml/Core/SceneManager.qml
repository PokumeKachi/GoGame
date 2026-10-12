import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {

    anchors.fill: parent

    StackView {
        id: stack

        anchors.fill: parent

        initialItem: "../Screens/LoginScreen.qml"
    }
}
