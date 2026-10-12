pragma Singleton

import QtQuick

QtObject {

    // Window

    readonly property color windowBackground: "#1E2126"

    // Panel

    readonly property color panelBackground: "#292D33"

    // Card

    readonly property color cardBackground: "#353A42"

    // Accent

    readonly property color primary: "#D7B26D"

    readonly property color secondary: "#8B6B3F"

    readonly property real primaryR: primary.r
    readonly property real primaryG: primary.g
    readonly property real primaryB: primary.b

    readonly property real secondaryR: secondary.r
    readonly property real secondaryG: secondary.g
    readonly property real secondaryB: secondary.b
    // Text

    readonly property color text: "#FFFFFF"

    readonly property color textGray: "#B8B8B8"

    readonly property color danger: "#FF4E4E"

    readonly property color success: "#49C26B"

}
