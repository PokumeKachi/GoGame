import QtQuick

Rectangle {
    id: root
    property double deadlineEpochMs: 0
    property int secondsLeft: Math.max(0, Math.ceil((deadlineEpochMs - Date.now()) / 1000))
    property bool urgent: secondsLeft > 0 && secondsLeft <= 8
    onDeadlineEpochMsChanged: secondsLeft = Math.max(0, Math.ceil((deadlineEpochMs - Date.now()) / 1000))
    width: 108; height: 42; radius: 21
    color: urgent ? "#a53535" : "#253247"
    border.width: 2; border.color: urgent ? "#ffcd76" : "#70829a"

    Timer { interval: 200; repeat: true; running: root.deadlineEpochMs > 0; onTriggered: root.secondsLeft = Math.max(0, Math.ceil((root.deadlineEpochMs - Date.now()) / 1000)) }
    Text { anchors.centerIn: parent; text: "⏳  " + root.secondsLeft + "s"; color: "white"; font.bold: true; font.pixelSize: 17 }
    SequentialAnimation on scale {
        running: root.urgent; loops: Animation.Infinite
        NumberAnimation { to: 1.09; duration: 360; easing.type: Easing.OutQuad }
        NumberAnimation { to: 1.0; duration: 360; easing.type: Easing.InQuad }
    }
}
