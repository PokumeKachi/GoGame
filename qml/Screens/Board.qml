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

    Rectangle {
        id: board
        anchors.centerIn: parent
        height: 608
        width: 608
        color: "#deb887"
        property int boardSize: 19
        property real margin: 0

        property var boardState: []

        Component.onCompleted: {
            initBoard();
        }

        function initBoard() {
            boardState = game.board_state;
        }

        Canvas {
            id: gridCanvas
            anchors.fill: parent
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
                var hoshiPoints = [];

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
            onClicked: function (mouse) {
                var usableWidth = width - (2 * boardRoot.margin);
                var usableHeight = height - (2 * boardRoot.margin);
                var stepX = usableWidth / (boardRoot.boardSize - 1);
                var stepY = usableHeight / (boardRoot.boardSize - 1);

                var col = Math.round((mouse.x - boardRoot.margin) / stepX);
                var row = Math.round((mouse.y - boardRoot.margin) / stepY);

                if (col >= 0 && col < boardRoot.boardSize && row >= 0 && row < boardRoot.boardSize) {
                    var index = row * boardRoot.boardSize + col;

                    var currentVal = boardState[index];
                    if (currentVal === 0) {
                        var temp = boardState.slice();
                        temp[index] = 1;
                        boardState = temp;
                    }
                }
            }
        }

        Repeater {
            model: boardRoot.boardSize * boardSize
            delegate: Item {
                id: stoneDelegate
                property int cellIndex: index
                property int stateValue: boardRoot.boardState[cellIndex] !== undefined ? boardRoot.boardState[cellIndex] : 0

                visible: stateValue !== 0

                width: {
                    let usableWidth = boardRoot.width - (2 * boardRoot.margin);
                    return (usableWidth / (boardRoot.boardSize - 1)) * 0.9;
                }
                height: width

                x: {
                    let usableWidth = boardRoot.width - (2 * boardRoot.margin);
                    let stepX = usableWidth / (boardRoot.boardSize - 1);
                    let col = cellIndex % boardRoot.boardSize;
                    return boardRoot.margin + col * stepX - width / 2;
                }

                y: {
                    let usableHeight = boardRoot.height - (2 * boardRoot.margin);
                    let stepY = usableHeight / (boardRoot.boardSize - 1);
                    let row = Math.floor(cellIndex / boardRoot.boardSize);
                    return boardRoot.margin + row * stepY - height / 2;
                }

                Rectangle {
                    anchors.fill: parent
                    radius: width / 2
                    color: stoneDelegate.stateValue === 1 ? "#111111" : "#f0f0f0"
                    border.color: stoneDelegate.stateValue === 1 ? "#000000" : "#cccccc"
                    border.width: 1

                    scale: 0.95
                }
            }
        }
    }
}
