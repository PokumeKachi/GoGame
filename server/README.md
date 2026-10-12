# GoGame WebSocket server

Server C++/Qt 6 chạy console, độc lập với ứng dụng QML. Cần Qt Core, Network và WebSockets.

```bash
cmake -S server -B build/server
cmake --build build/server --parallel 3
./build/server/appserver --port 14509 --data-file server/data/state.json
```

Chạy các lệnh trên từ thư mục gốc dự án. Server nghe tại `ws://127.0.0.1:14509`. Nếu bỏ `--data-file`, mặc định là `data/state.json` tính từ thư mục đang chạy. File chưa có được tạo với nội dung `{}`. File đã có phải chứa JSON object hợp lệ; lỗi đọc hoặc JSON hỏng khiến server dừng và giữ nguyên file.

## Giao thức

Gửi text message chứa JSON object theo dạng `{ "type": "...", "payload": { ... } }`.

Đọc file:

```json
{"type":"ReadData","payload":{}}
```

Phản hồi:

```json
{"type":"Data","payload":{"data":{}}}
```

Ghi file:

```json
{"type":"WriteData","payload":{"data":{"board":[0,-1,1],"moveCount":3,"currentPlayer":1,"time":{"black":600,"white":590}}}}
```

`payload.data` phải là JSON object. Mỗi yêu cầu ghi **thay toàn bộ nội dung file**; dữ liệu ví dụ trên chỉ minh họa giao thức, chưa phải trạng thái bàn cờ 19×19 đầy đủ. Ghi thành công trả về:

```json
{"type":"DataWritten","payload":{"ok":true}}
```

Lỗi trả về dạng:

```json
{"type":"Error","payload":{"code":"InvalidPayload","message":"payload.data must be a JSON object"}}
```

Các mã lỗi: `InvalidJson`, `InvalidPayload`, `UnknownType`, `ReadFailed`, `WriteFailed`, `UnsupportedMessage`. Binary message không được hỗ trợ. Giới hạn message nhận là 1 MiB.

Server xử lý yêu cầu tuần tự và ghi qua `QSaveFile` để thay file nguyên tử. Các client cùng dùng một file; yêu cầu ghi sau cùng thành công quyết định nội dung. Chỉ chạy một server cho mỗi file dữ liệu. Bản cơ bản này phục vụ local, chưa có xác thực, TLS, ghép trận hoặc kiểm tra luật game.

## Kết nối client hiện có

Client có biến môi trường `GOGAME_SERVER_URL`. Khi chạy client có thể đặt:

```bash
GOGAME_SERVER_URL=ws://127.0.0.1:14509 ./build/Desktop_Debug/appGoGame
```

Client tự kết nối lúc khởi động, mặc định dùng `ws://127.0.0.1:14509`. Màn hình đăng nhập hiển thị trạng thái/lỗi và có nút kết nối lại. Cần gửi `ReadData`/`WriteData` khi tích hợp đồng bộ dữ liệu. Các yêu cầu Auth/Matchmaking hiện có chưa được server cơ bản này xử lý.

## Kiểm tra

```bash
ctest --test-dir build/server --output-on-failure
```

Bài kiểm tra dùng port localhost 24509 và thư mục dữ liệu tạm, kiểm tra đọc/ghi qua WebSocket, hai client, dữ liệu tồn tại sau restart, từ chối message sai và bảo toàn file JSON hỏng. Có thể tắt build bài kiểm tra bằng `-DBUILD_TESTING=OFF`.

`main.qml` là file khung có sẵn; executable console không nạp file này.
