pragma Singleton

import QtQuick

QtObject {
    readonly property int instant: 80
    readonly property int fast: 140
    readonly property int medium: 220
    readonly property int slow: 360

    readonly property int _Fast: fast
    readonly property int _Medium: medium
    readonly property int _Slow: slow
}
