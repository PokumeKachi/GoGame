import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../Components/controls"
import "../Components/layout"
import "../Core"
import GoGame

Rectangle {
    implicitWidth: 1280
    implicitHeight: 720

    Row {
        anchors.top: parent.top
        anchors.topMargin: 12
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 12

        Text {
            text: gameManager.currentPlayer === 0
                  ? qsTr("Black to play")
                  : qsTr("White to play")
            font.pixelSize: 20
        }

        Button {
            text: qsTr("Pass")
            onClicked: gameManager.pass_turn()
        }
    }

    Rectangle {
        id: board
        anchors.centerIn: parent
        width: Math.max(0, Math.min(608, parent.width - 32, parent.height - 80))
        height: width
        color: "#deb887"
        property int boardSize: 19
        property real margin: 20
        property var boardState: gameManager.boardState

        Canvas {
            id: gridCanvas
            anchors.fill: parent
            onWidthChanged: requestPaint()
            onHeightChanged: requestPaint()
            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);

                var usableWidth = width - (2 * board.margin);
                var usableHeight = height - (2 * board.margin);
                var stepX = usableWidth / (board.boardSize - 1);
                var stepY = usableHeight / (board.boardSize - 1);

                ctx.strokeStyle = "#222222";
                ctx.lineWidth = 1;

                ctx.beginPath();
                for (let i = 0; i < board.boardSize; i++) {
                    let y = board.margin + i * stepY;
                    ctx.moveTo(board.margin, y);
                    ctx.lineTo(width - board.margin, y);

                    let x = board.margin + i * stepX;
                    ctx.moveTo(x, board.margin);
                    ctx.lineTo(x, height - board.margin);
                }
                ctx.stroke();

                drawHoshi(ctx, stepX, stepY);
            }

            function drawHoshi(ctx, stepX, stepY) {
                ctx.fillStyle = "#222222";
                var hoshiPoints = [3, 9, 15];

                if (board.boardSize === 19) {
                    hoshiPoints = [3, 9, 15];
                } else if (board.boardSize === 13) {
                    hoshiPoints = [3, 6, 9];
                } else if (board.boardSize === 9) {
                    hoshiPoints = [2, 4, 6];
                }

                for (let r = 0; r < hoshiPoints.length; r++) {
                    for (let c = 0; c < hoshiPoints.length; c++) {
                        let gx = hoshiPoints[c];
                        let gy = hoshiPoints[r];
                        let px = board.margin + gx * stepX;
                        let py = board.margin + gy * stepY;

                        ctx.beginPath();
                        ctx.arc(px, py, 4, 0, Math.PI * 2);
                        ctx.fill();
                    }
                }
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: function (mouse) {
                var usableWidth = width - (2 * board.margin);
                var usableHeight = height - (2 * board.margin);
                var stepX = usableWidth / (board.boardSize - 1);
                var stepY = usableHeight / (board.boardSize - 1);

                var col = Math.round((mouse.x - board.margin) / stepX);
                var row = Math.round((mouse.y - board.margin) / stepY);

                if (col >= 0 && col < board.boardSize && row >= 0 && row < board.boardSize) {
                    gameManager.update_pieces(row, col);
                }
            }
        }

        Repeater {
            model: board.boardSize * board.boardSize

            delegate: Item {
                id: stoneDelegate
                property int cellIndex: index

                property int stateValue: board.boardState && board.boardState[cellIndex] !== undefined ? board.boardState[cellIndex] : -1

                visible: stateValue !== -1

                width: {
                    let usableWidth = board.width - (2 * board.margin);
                    return (usableWidth / (board.boardSize - 1)) * 0.9;
                }
                height: width

                x: {
                    let usableWidth = board.width - (2 * board.margin);
                    let stepX = usableWidth / (board.boardSize - 1);
                    let col = cellIndex % board.boardSize;
                    return board.margin + col * stepX - width / 2;
                }

                y: {
                    let usableHeight = board.height - (2 * board.margin);
                    let stepY = usableHeight / (board.boardSize - 1);
                    let row = Math.floor(cellIndex / board.boardSize);
                    return board.margin + row * stepY - height / 2;
                }

                Rectangle {
                    anchors.fill: parent
                    radius: width / 2

                    color: stoneDelegate.stateValue === 0 ? "#111111" : "#f0f0f0"
                    border.color: stoneDelegate.stateValue === 0 ? "#000000" : "#cccccc"
                    border.width: 1

                    scale: 0.95
                }
            }
        }
    }
}
