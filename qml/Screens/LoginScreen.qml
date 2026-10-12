import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../Components/controls"
import "../Components/layout"
import "../Core"
import GoGame

Rectangle {
    Rectangle {
        id: root

        implicitWidth: 1280
        implicitHeight: 720

        property bool isRegisterMode: false

        // Gradient
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: "#0F172A"
                }
                GradientStop {
                    position: 0.4
                    color: "#1E1B4B"
                }
                GradientStop {
                    position: 1.0
                    color: "#090D16"
                }
            }
        }

        // Ambient Glow
        Rectangle {
            width: 480
            height: 480
            radius: 240
            anchors.centerIn: parent
            color: Qt.rgba(Theme.primaryR, Theme.primaryG, Theme.primaryB, 0.08)
        }

        ColumnLayout {
            // anchors.centerIn: parent
            x: 380
            y: 50
            spacing: Spacing.xl
            width: 480

            // Title
            ColumnLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: Spacing.xs

                GameText {
                    text: "✦ Go Game ✦"
                    type: GameText.Display
                    color: "white"
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                GameText {
                    text: "Wanna be a Go master? You choose the wrong place"
                    type: GameText.Caption
                    color: "white"
                    font.italic: true
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Kết nối lại server")
                visible: !networkClient.connected
                onClicked: networkClient.connectToServer(networkClient.serverUrl)
            }

            // Login / Register Card Panel
            GamePanel {
                Layout.fillWidth: true
                implicitHeight: 480
                radius: Radius.xl

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Spacing.xl
                    spacing: Spacing.md

                    GameText {
                        text: root.isRegisterMode ? "Create Account" : "Welcome Back"
                        type: GameText.Heading
                        color: "white"
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: Spacing.xxs

                        GameText {
                            text: "Username"
                            type: GameText.Caption
                            color: "white"
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 56
                            radius: Radius.md
                            color: Qt.rgba(0, 0, 0, 0.35)
                            border.width: userInput.activeFocus ? 2 : 1
                            border.color: userInput.activeFocus ? Theme.primary : "white"

                            TextInput {
                                id: userInput
                                anchors.fill: parent
                                anchors.margins: Spacing.md
                                color: Theme.text
                                font.pixelSize: Typography.body
                                font.family: Typography.family
                                clip: true
                                verticalAlignment: Text.AlignVCenter

                                Text {
                                    text: "Enter username..."
                                    color: Qt.rgba(1, 1, 1, 0.30)
                                    visible: !userInput.text && !userInput.activeFocus
                                    font: userInput.font
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: Spacing.xxs

                        GameText {
                            text: "Password"
                            type: GameText.Caption
                            color: "white"
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 56
                            radius: Radius.md
                            color: Qt.rgba(0, 0, 0, 0.35)
                            border.width: passInput.activeFocus ? 2 : 1
                            border.color: passInput.activeFocus ? Theme.primary : Qt.rgba(1, 1, 1, 0.15)

                            TextInput {
                                id: passInput
                                anchors.fill: parent
                                anchors.margins: Spacing.md
                                color: Theme.text
                                font.pixelSize: Typography.body
                                font.family: Typography.family
                                echoMode: TextInput.Password
                                clip: true
                                verticalAlignment: Text.AlignVCenter

                                Text {
                                    text: "Enter password..."
                                    color: Qt.rgba(1, 1, 1, 0.30)
                                    visible: !passInput.text && !passInput.activeFocus
                                    font: passInput.font
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }

                    // Status / Error Message
                    GameText {
                        id: statusLabel
                        text: authManager.statusMessage
                        type: GameText.Caption
                        color: (authManager.statusMessage.indexOf("Error") >= 0 || authManager.statusMessage.indexOf("Invalid") >= 0 || authManager.statusMessage.indexOf("already") >= 0 || authManager.statusMessage.indexOf("empty") >= 0) ? Theme.danger : Theme.primary
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                    }

                    // Main Action Button (Login or Register)
                    GameButton {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 48
                        text: root.isRegisterMode ? "Sign Up" : "Log In"

                        onClicked: {
                            if (root.isRegisterMode) {
                                authManager.registerUser(userInput.text, passInput.text);
                            } else {
                                authManager.login(userInput.text, passInput.text);
                            }
                        }
                    }

                    // Mode Toggle Button (Switch between Login / Register)
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: Spacing.xs

                        GameText {
                            text: root.isRegisterMode ? "Already have an account?" : "Don't have an account?"
                            type: GameText.Caption
                            color: "white"
                        }

                        GameText {
                            text: root.isRegisterMode ? "Log In" : "Sign Up"
                            type: GameText.Caption
                            color: "white"
                            font.bold: true

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    root.isRegisterMode = !root.isRegisterMode;
                                }
                            }
                        }
                    }
                }
            }
        }

        Connections {
            target: authManager

            function onLoginSuccess(username) {
                console.log("Logged in successfully as:", username);
                stack.replace("./MainMenu.qml");
            }

            function onRegisterSuccess(username) {
                console.log("Registered account:", username);
            }
        }
    }
}
