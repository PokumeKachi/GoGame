import QtQuick

Rectangle {
    property color backgroundColor: "#101722"
    color: backgroundColor
    gradient: Gradient {
        GradientStop { position: 0; color: "#1c2b42" }
        GradientStop { position: 0.55; color: "#252436" }
        GradientStop { position: 1; color: "#101722" }
    }
    Rectangle { anchors.fill: parent; anchors.margins: 16; radius: 18; color: "transparent"; border.width: 1; border.color: "#44d7b26d" }
}
