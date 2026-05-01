#include "HardwareSerial.h"
// App.cpp
#include "App.h"
#include <BlynkSimpleEsp32.h>

uint8_t fanState = 0;// Lưu trạng thái quạt (0: tắt, 1: bật)
uint8_t buzzState = 0;// Lưu trạng thái còi (0: tắt, 1: bật)
uint8_t modeState = 1;// Lưu chế độ hiện tại (0: Manual, 1: Auto)

// Global instance (required for Blynk callbacks)
App app;// Tạo đối tượng app từ lớp App

// === BLYNK CALLBACKS ===
BLYNK_WRITE(VIRTUAL_FAN)// Lắng nghe sự kiện từ chân ảo (Virtual Pin). Ví dụ, nếu trên App bạn cài đặt nút Quạt là chân V1, thì VIRTUAL_FAN trong code sẽ được định nghĩa là V1.
{
  int p = param.asInt();// Đọc giá trị từ nút nhấn trên App (0 hoặc 1)// p=1 
  app.setFanState(p);// Gọi hàm điều khiển quạt và dua gái trị p theo
}

BLYNK_WRITE(VIRTUAL_BUZZER)// chương trình nhận biết giao tiếp giữa app và esp32
{
  int p = param.asInt();
  app.setBuzzerState(p);
}

BLYNK_WRITE(VIRTUAL_MODE)
{
  int p = param.asInt();// p nhap từ app
  Serial.print("[BLYNK] VIRTUAL_MODE = ");
  Serial.println(p);

  ControlMode newMode = (p != 0) ? MODE_AUTO : MODE_MANUAL;// Nếu p khác 0 thì là AUTO
  app.setMode(newMode);
  modeState = newMode;
  Serial.print("[APP] Mode set to: ");
  Serial.println(app.getMode() == MODE_AUTO ? "AUTO" : "MANUAL");
  //MODE_AUTO (Tự động): ESP32 tự quyết định bật quạt/còi dựa trên cảm biến
  //MODE_MANUAL (Thủ công): Bạn tự bật/tắt quạt và còi từ giao diện Blynk.
  //Các hàm setMode, getMode dùng để thay đổi và kiểm tra trạng thái này
}

BLYNK_CONNECTED()
{
  Serial.println("[BLYNK] Connected - Syncing state...");
  app.syncAllToBlynk(); // Custom sync function
}

// === CONSTRUCTOR ===
App::App()
    : temperature(0), humidity(0), coDetected(0), fireDetected(false),//Khởi tạo giá trị ban đầu cho các biến cảm biến bằng 0 khi chương trình vừa chạy.
      mode(MODE_AUTO), lastSentMode(MODE_AUTO)//Khởi tạo giá trị ban đầu cho các biến cảm biến bằng 0 khi chương trình vừa chạy.
{
  // Default states
}

// === KHỞI TẠO ===
void App::begin()// lhoi73 tạo ban đầu và chờ kết nối vs app
{
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_BUZZER, OUTPUT);
  digitalWrite(RELAY_FAN, LOW);
  digitalWrite(RELAY_BUZZER, LOW);

  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);//BLYNK_AUTH_TOKEN: Đây là cái "Chứng minh thư" (Token) mà bạn đã lấy từ App. Nó giúp Server biết thiết bị này là của ai và thuộc dự án nào.
  //"blynk.cloud": Đây là địa chỉ Server (Máy chủ) toàn cầu của Blynk. ESP32 sẽ gửi dữ liệu đến địa chỉ internet này.
  //80: Đây là Cổng (Port) giao tiếp. Cổng 80 là cổng tiêu chuẩn dành cho các kết nối mạng không mã hóa (HTTP).
  //Thông thường người ta dùng Blynk.begin(). Nhưng trong code của bạn dùng .config là vì bạn muốn tách rời việc kết nối WiFi và kết nối Blynk. Việc này giúp code linh hoạt hơn, ví dụ: nếu WiFi mất, nó sẽ không làm treo toàn bộ chương trình mà chỉ dừng việc gửi dữ liệu lên App.
  Blynk.connect();
// khi mà esp32 kết nối dc với wifi sẽ dùng 2 dòng lệnh này để kết nối vs sever app  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80) và  Blynk.connect();
  // Wait for connection with timeout
  uint32_t start = millis();
  while (!Blynk.connected() && millis() - start < 10000)
  {
    Blynk.run();
    delay(100);
  }

  if (Blynk.connected())
  {
    Serial.println("[BLYNK] Connected successfully");
    syncAllToBlynk();
  }
  else
  {
    Serial.println("[BLYNK] Connection failed");
  }
}

// === CẬP NHẬT TRONG LOOP ===
void App::update()
{
  Blynk.run();// Duy trì kết nối với Server
  sendToBlynk();// Đẩy dữ liệu lên điện thoại
}

// === CẬP NHẬT CẢM BIẾN ===
void App::updateSensor(float temp, float humi, int co, bool fire)//Hàm này nhận kết quả từ các file đọc cảm biến khác, rồi "cất" vào các biến bên trong lớp App để chuẩn bị gửi đi
{
  temperature = temp;// ca bien nhiet do
  humidity = humi;// cam bien do ẩm
  coDetected = co;// cam biến co2
  // Serial.println(coDetected);
  fireDetected = fire;// cam bien hong ngoai
}

// === GỬI DỮ LIỆU LÊN BLYNK ===
void App::sendToBlynk()
{
  Blynk.virtualWrite(VIRTUAL_TEMP, temperature);// Gửi nhiệt độ lên App
  Blynk.virtualWrite(VIRTUAL_HUMI, humidity);// Gửi do ẩm lên app
  Blynk.virtualWrite(VIRTUAL_CO, coDetected);// Gửi nồng độ CO lên App
  Blynk.virtualWrite(VIRTUAL_FIRE, fireDetected ? 1 : 0);// Gửi trạng thái lửa

  // Only send mode when it changes
  if (mode != lastSentMode)
  {
    Blynk.virtualWrite(VIRTUAL_MODE, mode == MODE_AUTO ? 1 : 0);
    lastSentMode = mode;
    modeState = mode;
    Serial.printf("[SYNC] Mode sent: %s\n", mode == MODE_AUTO ? "AUTO" : "MANUAL");
  }

  Blynk.virtualWrite(VIRTUAL_FAN, digitalRead(RELAY_FAN));// Đẩy dữ liệu lên điệm thoại
  Blynk.virtualWrite(VIRTUAL_BUZZER, digitalRead(RELAY_BUZZER));// Đẩy dữ liệu lên điện thoại
}

// === ĐỒNG BỘ TOÀN BỘ KHI KẾT NỐI ===
void App::syncAllToBlynk()
{
  Blynk.virtualWrite(VIRTUAL_TEMP, temperature);
  Blynk.virtualWrite(VIRTUAL_HUMI, humidity);
  Blynk.virtualWrite(VIRTUAL_CO, coDetected);
  Blynk.virtualWrite(VIRTUAL_FIRE, fireDetected);
  Blynk.virtualWrite(VIRTUAL_MODE, modeState);
  Blynk.virtualWrite(VIRTUAL_FAN, digitalRead(RELAY_FAN));
  Blynk.virtualWrite(VIRTUAL_BUZZER, digitalRead(RELAY_BUZZER));
  lastSentMode = mode; // Prevent duplicate send
}

// === SETTER METHODS ===
void App::setFanState(int state)// state là giá trị app gửi về 0 hoặc 1 vs ấn bật quạt digitalWrite(RELAY_FAN, 1)
{
  digitalWrite(RELAY_FAN, state);// Xuất lệnh điện áp ra chân Relay để bật/tắt quạt là state =1 thì bật chân cảu quạt
  Blynk.virtualWrite(VIRTUAL_FAN, state);// Cập nhật lại trạng thái nút bấm trên App
  fanState = state;
  Serial.print("setFanState set to: ");//In ra màn hình dòng chữ "setFanState set to: " nhưng không xuống dòng
  Serial.println(fanState ? "On" : "Off");//In kết quả ("On" hoặc "Off") ra sau dòng chữ trên và nhảy xuống dòng tiếp theo
  //fanState: Đây là điều kiện kiểm tra. (Trong C++, số 1 tương đương với true, số 0 tương đương với false).
  //? "On": Nếu fanState là 1 (Đúng), nó sẽ chọn chữ "On".
  //: "Off": Nếu fanState là 0 (Sai), nó sẽ chọn chữ "Off".
  // giả sữ bật kết quả setFanState set to: On thì nó trả về phần Serial Monitor trên máy tính do có lệnh Serial.print hoặc Serial.printl
  // còn trả về oled khi có lệnh display.print, oledDisplay.draw..., oledDisplay.displayStatus()
  // còn trả về app thì với rờ le thì Blynk.virtualWrite() 
  // còn trả về app với thông số nhiệt độ cập nhật liên tục trong hàm sendToBlynk(), code sẽ liên tục "đẩy" dữ liệu về App:


}

void App::setBuzzerState(int state)
{
  digitalWrite(RELAY_BUZZER, state);
  Blynk.virtualWrite(VIRTUAL_BUZZER, state);
  buzzState = state;
  Serial.print("setBuzzerState set to: ");
  Serial.println(buzzState ? "On" : "Off");
}

void App::setMode(ControlMode newMode)
{
  if (mode != newMode)
  {
    mode = newMode;
    Serial.print("setMode set to: ");
    Serial.println(modeState ? "AUTO" : "MANUAL");
  }
}
void App::setLastSentMode()
{
  // setMode(MODE_AUTO);
  mode = MODE_AUTO;
  lastSentMode = MODE_MANUAL;
  Serial.println(mode == MODE_AUTO ? "AUTO" : "MANUAL");
  Serial.println(lastSentMode == MODE_AUTO ? "AUTO" : "MANUAL");
}
ControlMode App::getMode() const
{
  return mode;
}

ControlMode App::getLastSentMode() const
{
  return lastSentMode;
}