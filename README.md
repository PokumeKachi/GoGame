# GoGame

Dự án game cờ vây dùng C++ và Qt 6/QML. Giao diện nằm trong `qml/`, logic game và kết nối mạng nằm trong `src/`. Điểm khởi chạy là `main.cpp`, nạp giao diện `main.qml` và cung cấp các đối tượng game, xác thực, ghép trận, mạng cho QML.

README ghi lại cấu trúc hiện tại và kế hoạch phát triển; các mục dự định bên dưới chưa được xem là tính năng đã hoàn thiện. `src/Core/AI.h` và `AI.cpp` hiện mới là file khung.

## 1. Cấu trúc file hiện tại

Cập nhật ngày 12/10/2026. Liệt kê toàn bộ file mã nguồn, tài nguyên và thư mục rỗng hiện có. Nội dung `.git/`, `build/` và `.qtcreator/` được thu gọn vì là dữ liệu quản lý phiên bản, sản phẩm build và cấu hình IDE sinh tự động; các thư mục này vẫn được đánh dấu trong cây.

```text
GoGame/
├── .agents/
├── .aws/
├── .codex/
├── .git/  [nội dung Git / file sinh tự động, thu gọn]
├── .qtcreator/  [nội dung Git / file sinh tự động, thu gọn]
├── assets/
│   ├── fonts/
│   │   └── genshin.ttf
│   ├── sounds/
│   └── stones/
├── build/  [nội dung Git / file sinh tự động, thu gọn]
├── qml/
│   ├── Components/
│   │   ├── battle/
│   │   │   ├── CountdownClock.qml
│   │   │   └── ElementPointBar.qml
│   │   ├── cards/
│   │   │   ├── Card.qml
│   │   │   ├── CharacterCard.qml
│   │   │   └── SupportCard.qml
│   │   ├── common/
│   │   │   ├── Loading.qml
│   │   │   └── Popup.qml
│   │   ├── controls/
│   │   │   ├── GameButton.qml
│   │   │   ├── GameIcon.qml
│   │   │   ├── GameImage.qml
│   │   │   ├── GamePanel.qml
│   │   │   └── GameText.qml
│   │   └── layout/
│   │       ├── GameBackground.qml
│   │       └── GamePanel.qml
│   ├── Core/
│   │   ├── Constants.qml
│   │   ├── Motion.qml
│   │   ├── Radius.qml
│   │   ├── SceneManager.qml
│   │   ├── Spacing.qml
│   │   ├── Theme.qml
│   │   └── Typography.qml
│   ├── Screens/
│   │   ├── Board.qml
│   │   ├── LoginScreen.qml
│   │   ├── MainMenu.qml
│   │   └── SettingsScreen.qml
│   └── Themes/
├── server/
│   ├── data/
│   │   └── state.json
│   ├── tests/
│   │   └── Integration.cpp
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── main.cpp
│   └── main.qml
├── src/
│   ├── Core/
│   │   ├── AI.cpp
│   │   ├── AI.h
│   │   ├── Enums.cpp
│   │   ├── Enums.h
│   │   ├── Enumutils.cpp
│   │   ├── Enumutils.h
│   │   ├── Game.cpp
│   │   └── Game.h
│   ├── Data/
│   ├── Managers/
│   │   ├── AuthManager.cpp
│   │   ├── AuthManager.h
│   │   ├── MatchmakingManager.cpp
│   │   └── MatchmakingManager.h
│   ├── Models/
│   │   ├── Board.cpp
│   │   ├── Board.h
│   │   ├── UI.cpp
│   │   └── UI.h
│   └── Network/
│       ├── NetworkClient.cpp
│       ├── NetworkClient.h
│       ├── Protocol.cpp
│       └── Protocol.h
├── .gitignore
├── CMakeLists.txt
├── README.md
├── main.cpp
├── main.qml
└── users.sqlite
```

| Đường dẫn                                                      | Vai trò                                                                               |
| -------------------------------------------------------------- | ------------------------------------------------------------------------------------- |
| `CMakeLists.txt`                                               | Cấu hình executable `appGoGame`, module QML `GoGame` và tài nguyên.                   |
| `main.cpp`, `main.qml`                                         | Khởi tạo ứng dụng và giao diện gốc.                                                   |
| `qml/Screens/`                                                 | Các màn hình đăng nhập, menu chính, cài đặt và bàn đấu.                               |
| `qml/Core/`                                                    | Quản lý màn hình, theme, font, khoảng cách, bo góc, chuyển động và hằng số.           |
| `qml/Components/`                                              | Các thành phần giao diện dùng lại, chia theo battle, cards, common, controls, layout. |
| `src/Core/`                                                    | Logic game, enum, tiện ích enum và phần khung AI.                                     |
| `src/Managers/`                                                | Quản lý xác thực và ghép trận.                                                        |
| `src/Models/`                                                  | Các file mô hình Board và UI.                                                         |
| `server/` | Server WebSocket C++ độc lập, tài liệu giao thức, dữ liệu JSON và kiểm thử integration. |
| `src/Network/`                                                 | Client mạng và giao thức trao đổi.                                                    |
| `assets/fonts/`                                                | Font `genshin.ttf`.                                                                   |
| `assets/sounds/`, `assets/stones/`, `qml/Themes/`, `src/Data/` | Thư mục hiện còn trống.                                                               |
| `users.sqlite`                                                 | File SQLite hiện có, dung lượng 0 byte tại thời điểm kiểm tra.                        |
| `.agents/`, `.aws/`, `.codex/`                                 | Thư mục môi trường hiện có, đang trống.                                               |
| `build/`, `.qtcreator/`, `.git/`                               | Dữ liệu build, IDE và Git; không phải mã nguồn tính năng.                             |

Có hai file `GamePanel.qml` ở `controls/` và `layout/`; cây thư mục giữ nguyên cả hai.

### Trạng thái hiện tại

- Đã tạo server sơ khai bằng C++/Qt, chạy console độc lập với client, gửi/nhận JSON qua WebSocket và đọc/ghi file JSON.
- Đã sửa một số lỗi component/màn hình do template: Board dùng `gameManager` và property bàn cờ cho QML; chuyển đích màn hình từ `DeckSelectScreen.qml` sang `Board.qml`; sửa URL reconnect và thêm nút kết nối lại trên màn hình đăng nhập.
- Việc dọn template chưa hoàn tất: `MainMenu.qml` vẫn tham chiếu `deckManager`, đối tượng chưa được cung cấp từ `main.cpp`.

### Server: đã có và còn thiếu

| Hạng mục | Trạng thái hiện tại |
| --- | --- |
| Server độc lập | Đã có target `appserver`, Qt Core/Network/WebSockets, nghe `ws://127.0.0.1:14509` mặc định. |
| Giao thức JSON | Đã có text message `{type,payload}`: `ReadData` trả `Data`, `WriteData` trả `DataWritten`; lỗi trả `Error`. |
| Lưu trữ | Đã đọc/ghi JSON object bằng `--data-file`, tạo `{}` khi chưa có file, ghi nguyên tử bằng `QSaveFile`. Mỗi lần ghi thay toàn bộ nội dung; các client dùng chung một file. |
| Kiểm tra dữ liệu | Đã từ chối JSON/payload sai, loại yêu cầu không hỗ trợ và binary message; giới hạn message 1 MiB. Chưa kiểm tra schema trạng thái trận hoặc luật cờ. |
| Kết nối client | Đã tự kết nối khi khởi động, hỗ trợ `GOGAME_SERVER_URL` và reconnect. Kết nối thành công chưa có nghĩa là đăng nhập/ghép trận hoạt động. |
| Kiểm thử | Có `server/tests/Integration.cpp` và CTest kiểm tra đọc/ghi, nhiều client, restart, dữ liệu sai và reconnect. Kết quả chạy trước được ghi trong nhật ký; lần cập nhật README này không chạy lại test. |
| Auth và matchmaking | Chưa có xử lý đăng ký/đăng nhập, phiên người dùng, hàng chờ và ghép trận. Các yêu cầu tương ứng hiện chưa được server hỗ trợ. |
| Đồng bộ trận đấu | Chưa tích hợp Board với `ReadData`/`WriteData`; chưa có phòng trận riêng, kiểm tra lượt/nước đi ở server hoặc cập nhật tự động tới đối thủ. |
| Trạng thái và thời gian | Chưa thống nhất schema bàn cờ, số nước, người đến lượt, thời gian; chưa có đồng hồ do server quản lý và khôi phục phiên trận. |
| Luật và kết thúc trận | Chưa có kiểm tra luật Trung/Nhật ở server, tính điểm, bỏ lượt, đầu hàng và kết quả trận. |
| Replay | Chưa có lưu/xem lại tối đa 5 ván gần nhất. |
| Triển khai ngoài local | Hiện chỉ nghe localhost; chưa có TLS, xác thực hay phân quyền truy cập dữ liệu. |

Chi tiết giao thức và kiểm thử: [server/README.md](server/README.md). `server/main.qml` là file khung; server console không nạp file này.

### Chạy server trong terminal riêng

Từ thư mục gốc `GoGame`, mở **một terminal riêng** và chạy lần lượt:

```bash
cmake -S server -B build/server
cmake --build build/server --parallel 3
./build/server/appserver --data-file server/data/state.json
```

**Để terminal này chạy nguyên đó trong lúc sử dụng client.** Server chạy ở foreground; nhấn `Ctrl+C` để dừng khi không còn sử dụng. Mọi thao tác chơi còn lại thực hiện trên client, mở bằng Qt Creator hoặc terminal khác theo hướng dẫn bên dưới. Cần Qt 6.2 trở lên với Core, Network và WebSockets để build server; nếu CMake chưa tìm được Qt, thêm `-DCMAKE_PREFIX_PATH=/duong/dan/toi/Qt` vào lệnh cấu hình.

Client mặc định kết nối `ws://127.0.0.1:14509`. Nếu client đã mở trước server, dùng nút **Kết nối lại server** trên màn hình đăng nhập. Có thể đặt URL khi mở client:

```bash
GOGAME_SERVER_URL=ws://127.0.0.1:14509 ./build/Desktop_Debug/appGoGame
```

### Build client

Cần CMake từ 3.16, trình biên dịch C++ và Qt 6 có các module Quick/QML, Sql, Network, WebSockets. Cấu hình hiện tại yêu cầu Qt 6.2 trở lên cho Quick và Sql.

```bash
cmake -S . -B build/local -DCMAKE_PREFIX_PATH=/duong/dan/toi/Qt
cmake --build build/local --parallel 3
./build/local/appGoGame
```

Thay đường dẫn Qt theo máy; với bộ sinh đa cấu hình, đường dẫn executable tùy cấu hình build. Có thể mở `CMakeLists.txt` bằng Qt Creator và chọn kit Qt tương ứng. Đây là hướng dẫn từ cấu hình dự án, chưa xác nhận build trong lần cập nhật tài liệu này.

## 2. Những dự định trong tương lai

### 1. Hoàn thiện Board.qml trước

- [ ] Ưu tiên `qml/Screens/Board.qml`.
- [ ] Hoàn thiện UI bàn đấu: ô hiện tên người chơi, avatar và đồng hồ đếm ngược.

### 2. Đặt quân bằng hai lần nhấn

- [ ] Lần đầu tại vị trí hợp lệ: chỉ hiện quân cờ mờ, nhấp nháy (preview).
- [ ] Lần thứ hai tại cùng vị trí: xác nhận đặt quân.
- [ ] Cho phép bật/tắt chế độ này trong settings.

### 3. Phản hồi khi đặt sai vị trí

- [ ] Hiện chữ báo lỗi ở trên cùng màn hình.
- [ ] Không cho đặt quân, kể cả preview, ở vị trí không hợp lệ.
- [ ] Khoanh đỏ vị trí lỗi trong 0,5–1 giây.
- [ ] Khi triển khai, **thêm comment rõ ràng ngay đoạn xử lý khoanh đỏ và thời gian hiển thị** để dễ tìm, chỉnh và sửa lỗi.
- [ ] Phát âm thanh báo lỗi.

### 4. Luật Trung / Nhật

- [ ] Làm luật Trung trước.
- [ ] Cân nhắc luật Nhật sau vì triển khai phức tạp; chưa ưu tiên.

### 5. Animation khi ăn quân

- [ ] Thêm animation ăn quân ở giai đoạn sau vì khá phức tạp.

### 6. Tài nguyên âm thanh

- [ ] Tìm nhạc game.
- [ ] Tìm âm thanh đặt quân và âm thanh báo lỗi.

### 7. Settings trong trận

- [ ] Cho phép tùy chỉnh âm thanh.
- [ ] Cho phép bật/tắt chế độ preview.

### 8. Settings ngoài trận

- [ ] Đổi giao diện bàn đấu, avatar, khung avatar và danh hiệu.
- [ ] Có tùy chỉnh level và rank theo yêu cầu; cơ chế cụ thể sẽ xác định khi triển khai.
- [ ] Bao gồm mọi tùy chỉnh của settings trong trận.

### 9. Đấu với người qua JSON

- [ ] Trao đổi/lưu trạng thái trận bằng JSON gồm sân đấu hiện tại (trạng thái bàn cờ), số bước đã qua, người đang đến lượt và thời gian.
- [ ] Thống nhất định dạng dữ liệu trước khi triển khai đồng bộ trận đấu.

### 10. Xem lại ván đấu

- [ ] Lưu tối đa 5 ván gần nhất bằng JSON chứa các nước đã đi.
- [ ] Cho phép xem lại diễn biến ván đấu.
- [ ] Đánh giá nước đi làm sau vì cần AI.

### 11. AI đấu với máy

| Độ khó     | Dự định                                                                               |
| ---------- | ------------------------------------------------------------------------------------- |
| Dễ         | Minimax suy nghĩ nông, tính trước 1–2 nước.                                           |
| Trung bình | Minimax; độ sâu và ngân sách tính toán sẽ xác định khi thử nghiệm.                    |
| Khó        | Alpha-Beta Pruning kết hợp MCTS để tối ưu; hướng tới dùng mạng 6-block ONNX trên CPU. |

- [ ] Đo số phép tính và thời gian trên máy thực tế để mỗi nước AI không tốn quá 1–2 giây.
- [ ] Giới hạn song song tối đa 3 luồng CPU. Ý tưởng ban đầu dùng `ParallelOptions { MaxDegreeOfParallelism = 3 }`; đây là cú pháp C#, khi triển khai trong dự án C++/Qt cần cơ chế tương đương.
- [ ] MCTS giới hạn tối đa 1000 playouts và dừng khi hết ngân sách thời gian.
- [ ] Lớp bảo vệ Alpha-Beta nông ở depth 1 quét các tình huống nguy cấp như Atari, cứu quân; mục tiêu quét khoảng 1 ms cần được đo kiểm.
- [ ] Nếu có nước nguy cấp, trả về nước xử lý ngay; nếu thế trận hòa hoãn, chạy MCTS.
- [ ] Mạng 6-block `.onnx` chạy CPU: policy gợi ý 3–5 nước tốt nhất, value chấm điểm thế trận.

Luồng ý tưởng ban đầu:

```text
[ Người chơi đi 1 nước ]
             |
             v
[ Lớp bảo vệ: Alpha-Beta cắt tỉa nông (Depth 1) ]
[ Quét nhanh: Atari, cứu quân — mục tiêu 1 ms ]
             |
       +-----+--------------------+
       |                          |
Có nước nguy cấp          Thế trận hòa hoãn
       |                          |
       v                          v
[ AI đi luôn ]           [ Bộ não chính: MCTS ]
                         [ Tối đa 3 luồng CPU ]
                         [ Tối đa 1000 playouts ]
                                  |
                                  v
                         [ Mạng 6-block ONNX / CPU ]
                         [ Policy: gợi ý 3–5 nước ]
                         [ Value: chấm điểm thế trận ]
                                  |
                                  v
                         [ Trả về nước đi tối ưu ]
```

“Mát máy, 0% CPU” trong ý tưởng gốc là mong muốn giảm tải khi chọn được nước ngay; xử lý thực tế vẫn sử dụng CPU. Các mốc 1 ms, 1–2 giây, 3 luồng và 1000 playouts là mục tiêu/giới hạn thiết kế cần thử nghiệm, chưa phải kết quả benchmark.

### 12. Build mạng neuron 6b =)))

- [ ] Xây dựng và huấn luyện mạng neuron 6-block.
- [ ] Xuất mô hình `.onnx` để phục vụ policy/value trong AI.
- [ ] Thử nghiệm tốc độ suy luận CPU cùng ngân sách thời gian của MCTS.

## 3. Khai báo AI và nhật ký prompt

README này được soạn với sự hỗ trợ của AI (Codex) theo yêu cầu của chủ dự án. Lần cập nhật này chỉ tạo tài liệu, không triển khai các tính năng trong kế hoạch.

### Phạm vi lịch sử và quy ước cập nhật

Chỉ có prompt liên quan dự án đang hiện diện trong cuộc trò chuyện này để ghi lại. Không có nội dung các prompt ở những phiên trước nên chưa thể khẳng định đây là toàn bộ lịch sử dự án; không suy đoán hoặc tự dựng lại prompt cũ từ mã nguồn.

Theo yêu cầu của chủ dự án, từ đây mỗi lần AI xử lý prompt mới liên quan GoGame phải bổ sung **nguyên nội dung prompt**, ngày và kết quả thực hiện vào mục này, giữ nguyên các mục cũ. Prompt cũ được cung cấp thêm cũng phải được bổ sung, ghi rõ ngày nếu biết. Đây là quy ước cần thực hiện trong các lần làm việc tiếp theo, không phải cơ chế tự động thu thập các cuộc trò chuyện ngoài quyền truy cập.

### 12/10/2026 — Yêu cầu tạo README.md

Nội dung prompt của người dùng (chuẩn hóa khoảng trắng và xuống dòng từ định dạng tin nhắn):

```text
Viết README.md với những yêu cầu sau:

1. Đầy đủ cấu trục file hiện tại
2. Những dự định trong tương lai:
   1. Cần làm phần Board.qml trước, hoàn thiện UI gồm: Phần ô hiện tên người chơi, avatar, đồng hồ đếm ngược,
   2. Đặt quân cần ấn liên tiếp 2 lần, cụ thể ấn lần đầu chỉ hiện quân cờ mờ, nhấp nháy, ấn lần 2 để xác nhận đặt quân. Có thể bật tắt tùy chỉnh này trong settings.
   3. Tại các ô đặt vị trí sai sẽ báo lỗi bằng cách hiện chữ ở trên cùng màn hình, không cho đặt kể cả preview, khoanh đỏ ô đặt lỗi trong 0,5-1 giây(comment lại đoạn này để dễ fix), âm thanh báo lỗi.
   4. Luật Trung/Nhật(có thể sẽ làm nhưng rất phiền nên để làm luật Trung trước.
   5. Animation khi ăn quân (khá phiền nên để sau).
   6. Tìm tài nguyên nhạc game, âm thanh đặt quân.
   7. Trong phần setting trong trận, cho phép tùy chỉnh âm thanh, tắt mở chế độ preview.
   8. Trong phần setting ngoài trận, cho phép đổi giao diện bàn đấu, avatar, khung của avatar, danh hiệu, level, rank. Và mọi setting trong trận.
   9. Chế độ đấu với người hãy thực hiện qua JSON sẽ lưu: sân đấu hiện tại, số bước đã qua, người đi lượt hiện tại, thời gian.
   10. Chế độ xem lại ván đấu: lưu nhiều nhất 5 ván gần nhất lưu json các nước đã đi. Đánh giá nước đi (Tính năng làm sau do cần AI).
   11. AI độ khó của chế độ đấu với máy: Dễ(Suy nghĩ nông tính trước 1-2 nước dùng MinMax, Trung bình dùng MinMax, Khó Alpha-Beta Prune + MCTS để tối ưu (test trước số lượng phép tính cần sao cho các bước không tốn quá 1-2 giây) (giới hạn cpu nếu k cháy máy ParallelOptions { MaxDegreeOfParallelism = 3 }) ý tưởng chính là:

 [ Người chơi đi 1 nước ]
                          │
                          ▼
    [ Lớp Bảo Vệ: Alpha-Beta Cắt Tỉa Nông (Depth 1) ]
    (Quét nhanh 1ms các nước nguy cấp: Atari, Cứu quân)
                          │
            ┌─────────────┴─────────────┐
   Có nước nguy cấp             Thế trận hòa hoãn
            │                           │
            ▼                           ▼
      [ AI ĐI LUÔN ]             [ Bộ não chính: MCTS ]
   (Mát máy, 0% CPU)      ┌─────────────┴─────────────┐
                          │   Khóa cứng: 3 Luồng CPU  │
                          │   Khóa cứng: 1000 Playouts│
                          └─────────────┬─────────────┘
                                        ▼
                    [ Dùng Mạng 6-Block (.onnx) trên CPU ]
                    - Gợi ý 3-5 nước tốt nhất (Policy)
                    - Chấm điểm thế trận lập tức (Value)
                                        │
                                        ▼
                           [ Trả về nước đi tối ưu ]

12. Build cái mạng neuron 6b =)))

3) Khai báo AI: Hãy viết tất cả những gì tôi đã prompt cho bạn của dự án này kể cả hiện tại và sau này vô đây
```

Tài liệu tham khảo: https://www.studocu.vn/vn/document/dai-hoc-dien-luc/dien-luc-university/bao-cao-nhap-mon-tri-tue-nhan-tao/88382780?sid=a353be05-df77-4401-bae7-4597fba4fd601791775595
Kết quả: tạo README.md với cấu trúc hiện tại, đủ 12 mục kế hoạch và nhật ký prompt này. Đối chiếu cây mã nguồn/tài nguyên với hệ thống file; không chạy build hoặc test vì chỉ thay đổi tài liệu.

### 12/10/2026 — Cập nhật README về server và component template

Nội dung prompt của người dùng (chuẩn hóa xuống dòng):

```text
Viết readme.md:

1. Tạo xong server sơ khai, chạy được json
2. Fix 1 số component gây lỗi do template
3. Thêm hướng dẫn sử dụng server như sau: Mở 1 terminal riêg chạy "cmake -S server -B build/server
   cmake --build build/server --parallel 3
   ./build/server/appserver --data-file server/data/state.json" và để yên đó còn lại là client.
4. Cập nhật trạng thái của server: đã có, thiếu những gì.
```

Kết quả: cập nhật README.md hiện có, bổ sung server vào cây thư mục, ghi nhận server JSON sơ khai và các lỗi template đã sửa; thêm hướng dẫn giữ server chạy trong terminal riêng và mở client riêng. Đối chiếu mã server/client để liệt kê phần đã có và còn thiếu, bao gồm tham chiếu `deckManager` còn sót. Lần này chỉ chỉnh tài liệu; không sửa component, không chạy build hoặc test lại.


### 12/10/2026 — Resolve GitHub push conflict

Nội dung prompt của người dùng:

```text
relovse confict of github push
```

Kết quả: hợp nhất `origin/main` vào `main`, giữ lịch sử hai nhánh và sửa conflict trong `server/main.cpp`, `src/Network/NetworkClient.cpp`. Hai phiên bản chỉ khác định dạng khoảng trắng; giữ định dạng local, nhận các thay đổi remote về cấu hình build, tài liệu và test.

Kiểm tra merge: build client `build/Desktop_Debug` và build server sạch `build/merge-check-server` thành công; CTest integration đạt 1/1. Build server trong thư mục cũ gặp cache AUTOMOC lỗi thời; build sạch xác nhận mã hợp nhất hoạt động.
