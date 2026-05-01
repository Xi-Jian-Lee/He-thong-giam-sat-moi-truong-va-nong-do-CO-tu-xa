#include <stdint.h>
#ifndef APP_H
#define APP_H

// #define BLYNK_TEMPLATE_ID "TMPL6vkHHIQIO"
// #define BLYNK_TEMPLATE_NAME "Esp32 V10"
// #define BLYNK_AUTH_TOKEN "uQZ6oKrKNIlTfGpHiiOMWyGHcOHWqRJB"

#define BLYNK_TEMPLATE_ID "TMPL2-gqCaTnl"// là thứ giúp xác nhận và tìm ra giao diện đã thiết kế trên app
#define BLYNK_TEMPLATE_NAME "Esp32 V10"
#define BLYNK_AUTH_TOKEN "uLVQbG_drXJhbJSkXhxPvfGGUCvI6ntf"

#include <Arduino.h>
#include <WiFi.h>


// ====== WiFi Info ======
//#define WIFI_SSID "Thanh An"// ten wifi nho sua
//#define WIFI_PASS "linhtuyen78"// pass wifi nhớ sữa
#define WIFI_SSID "HTN2"// ten wifi nho sua
#define WIFI_PASS "LIENMINH"// pass wifi nhớ sữa

// ====== Hardware Pins ======
//#define RELAY_FAN 27// Quạt nối vào chân GPIO 27 khi có lệnh nó sẽ kích chân đó lên mức 1
//#define RELAY_BUZZER 14//Còi nối vào chân GPIO 14
#define RELAY_FAN 14// Quạt nối vào chân GPIO 14 khi có lệnh nó sẽ kích chân đó lên mức 1
#define RELAY_BUZZER 27//Còi nối vào chân GPIO 27
// ====== Virtual Pins ======
#define VIRTUAL_FAN V0// chân ảo gao tiếp qua lại vs app
#define VIRTUAL_BUZZER V1
#define VIRTUAL_FIRE V2
#define VIRTUAL_CO V3
#define VIRTUAL_TEMP V4// v4 là chân ảo không tồn tại trên mạch cứng khi ta lập trình thì khi ta muốn bật tắt gì đó nếu nhận được tín hiệu chân v2 thì bật tắt chân 27
#define VIRTUAL_HUMI V5
#define VIRTUAL_MODE V6

extern uint8_t fanState;
extern uint8_t buzzState;
extern uint8_t modeState;
//Từ khóa extern thông báo cho trình biên dịch rằng: "Đối tượng app và các biến trạng thái này đã được tạo ra ở chỗ khác (file .cpp), ở đây tôi chỉ khai báo lại để các file khác cùng dùng chung một nguồn dữ liệu đó
enum ControlMode {
  MODE_MANUAL,
  MODE_AUTO
};

class App {
public:
  App();
  void begin();
  void update();
  void updateSensor(float temp, float humi, int co, bool fire);

  void setFanState(int state);
  void setBuzzerState(int state);
  void setMode(ControlMode newMode);
  void setLastSentMode();
  void sendToBlynk();
  void syncAllToBlynk();
  ControlMode getMode() const;
  ControlMode getLastSentMode() const;
private:

  // Sensor data
  float temperature;
  float humidity;
  int coDetected;
  bool fireDetected;

  // Control state
  ControlMode mode;
  ControlMode lastSentMode;
};

extern App app;  // Declare global instance

#endif// dòng này giúp tránh việc nạp đi nạp lại nhiều lần file trong quá trình chạy



// có 3 ngưỡng khiến cho rờ le nhảy ngưỡng nhiệt độ lớn hơn 42 là báo
// ngưỡng gas là 1860
// ngưỡng lừa có hay không 1 hay 0
// trong app chỉ có nhiệt đô, độ ẩm, gas và độ ẩm có 100 thì sẽ ko báo