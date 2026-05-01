#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_GFX.h>// Thư viện đồ họa cơ bản (vẽ điểm, đường, chữ)
//#include <Adafruit_SH1106.h>
#include <Adafruit_SH110X.h>// Thư viện đặc thù cho chip điều khiển SH1106
#include <Wire.h>// Thư viện giao tiếp I2C
#include "BitMap.h"// File chứa các hình ảnh cột sóng Wi-Fi
//SDA = 21, SCL = 22: Đây là hai chân I2C mặc định của ESP32.
//0x3C: Địa chỉ I2C phổ biến nhất của màn hình OLED
//128x64: Độ phân giải màn hình.
//rst = -1: Nghĩa là không dùng chân Reset cứng (nối thẳng vào cực dương hoặc không dùng)
class OLED_Display {
public:
  OLED_Display(uint8_t sda = 21, uint8_t scl = 22, uint8_t address = 0x3C,
               uint8_t w = 128, uint8_t h = 64, int8_t rst = -1);

  bool begin();
  bool isReady() const;// (kiểm tra trạng thái)

  void clear();
  void update();

  void printText(const String &text, int x = 0, int y = 0, uint8_t size = 1);// in chữ tại ví trị chính xác
  void printTextCon(const char *text, uint8_t size = 1);// in chữ nối tiếp
  void printScrollText(const String &text, int x, int y, uint8_t size, bool rightToLeft = true);// Dự định in chữ chạy

  void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);// vẽ đưởng thẳng
  void drawScrollLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);// vẽ đường thẳng có hiệu ứng chạy 

  void starting();
  void Template();
  void initSys();
  void connectWifi();
  void displayStatus(const String &signalQuality);

private:
  uint8_t SDA, SCL, i2cAddress;
  int8_t resetPin;
  bool isInitialized;// Biến cờ để kiểm tra màn hình đã bật chưa
  uint8_t wifiState = 0;    // Lưu trạng thái nhấp nháy của cột sóng Wi-Fi
  //Adafruit_SH110X display; 
  Adafruit_SH1106G display;// Đối tượng màn hình thực tế từ thư viện Adafruit
};

extern const unsigned char wifi_Dot_bmp[], wifi_Bad_bmp[], wifi_Good_bmp[], wifi_Strong_bmp[];
//Dòng này báo cho trình biên dịch biết rằng các hình ảnh cột sóng Wi-Fi được định nghĩa ở file BitMap.h, lớp OLED_Display có quyền lấy chúng ra để sử dụng.
#endif