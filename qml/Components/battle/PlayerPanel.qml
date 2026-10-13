import QtQuick

Item {
    id: root

    property string playerName: ""
    property url avatarSource: ""
    property bool avatarOnRight: false
    property color frameColor: "#bda78c"
    readonly property real unit: height / 90

    implicitWidth: 420
    implicitHeight: 90

    Canvas {
        id: frame
        anchors.fill: parent
        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()
        Connections {
            target: root
            function onAvatarOnRightChanged() { frame.requestPaint() }
            function onFrameColorChanged() { frame.requestPaint() }
        }
        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()
            var u = root.unit
            ctx.scale(u, u)
            var w = width / u
            if (root.avatarOnRight) {
                ctx.translate(w, 0)
                ctx.scale(-1, 1)
            }
            // Scalloped end and inset gold outline match the reference nameplate.
            function outline(inset) {
                ctx.beginPath()
                ctx.moveTo(28, 3 + inset)
                ctx.lineTo(w - 40, 3 + inset)
                ctx.quadraticCurveTo(w - 20, 3 + inset, w - 16, 18)
                ctx.bezierCurveTo(w - 5, 20, w - 6, 30, w - 3 - inset, 35)
                ctx.quadraticCurveTo(w + 4 - inset, 44, w - 3 - inset, 53)
                ctx.bezierCurveTo(w - 6, 60, w - 5, 70, w - 16, 72)
                ctx.quadraticCurveTo(w - 20, 87 - inset, w - 40, 87 - inset)
                ctx.lineTo(28, 87 - inset)
                ctx.quadraticCurveTo(3 + inset, 87 - inset, 3 + inset, 64)
                ctx.lineTo(3 + inset, 26)
                ctx.quadraticCurveTo(3 + inset, 3 + inset, 28, 3 + inset)
                ctx.closePath()
            }
            outline(0)
            var gradient = ctx.createLinearGradient(0, 0, w, 90)
            gradient.addColorStop(0, "#252c35")
            gradient.addColorStop(0.6, "#343a43")
            gradient.addColorStop(1, "#242b33")
            ctx.fillStyle = gradient
            ctx.shadowColor = "#35000000"
            ctx.shadowBlur = 5
            ctx.shadowOffsetY = 3
            ctx.fill()
            ctx.shadowBlur = 0
            ctx.shadowOffsetY = 0
            ctx.strokeStyle = root.frameColor
            ctx.lineWidth = 2.5
            ctx.stroke()
            outline(4)
            ctx.strokeStyle = "#716a60"
            ctx.lineWidth = 1
            ctx.stroke()
        }
    }

    // Diamond ornament is placed beyond the scalloped edge.
    Rectangle {
        x: root.avatarOnRight ? -7 * root.unit : parent.width - 8 * root.unit
        anchors.verticalCenter: parent.verticalCenter
        width: 15 * root.unit
        height: width
        rotation: 45
        color: "transparent"
        border.width: 3 * root.unit
        border.color: root.frameColor
        Rectangle {
            anchors.centerIn: parent
            width: 5 * root.unit
            height: width
            color: root.frameColor
        }
    }

    Rectangle {
        id: avatarFrame
        x: root.avatarOnRight ? parent.width - width - 14 * root.unit : 14 * root.unit
        anchors.verticalCenter: parent.verticalCenter
        width: 78 * root.unit
        height: 78 * root.unit
        radius: 16 * root.unit
        color: "#1e252d"
        border.color: "#8d806d"
        border.width: root.unit

        Image {
            anchors.fill: parent
            anchors.margins: 7 * root.unit
            source: root.avatarSource
            fillMode: Image.PreserveAspectFit
            smooth: true
        }
    }

    Text {
        x: root.avatarOnRight ? 35 * root.unit : 106 * root.unit
        width: Math.max(0, root.width - 141 * root.unit)
        anchors.verticalCenter: parent.verticalCenter
        text: root.playerName
        color: "#f3ede1"
        font.family: "Noto Serif"
        font.pixelSize: 25 * root.unit
        font.bold: true
        elide: Text.ElideRight
        maximumLineCount: 1
        verticalAlignment: Text.AlignVCenter
        style: Text.Raised
        styleColor: "#252932"
    }
}
