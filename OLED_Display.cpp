#include "OLED_Display.h"
#include <Arduino.h>
#include <Wire.h>
// màn hình oại SH1106, kích thước 128x64
// Sửa hàm khởi tạo cho đúng chuẩn Adafruit_SH1106G (Dùng cho SH1106 I2C)
OLED_Display::OLED_Display(uint8_t sda, uint8_t scl, uint8_t address,
                           uint8_t w, uint8_t h, int8_t rst)
  : SDA(sda), SCL(scl), i2cAddress(address), resetPin(rst),
    display(128, 64, &Wire, rst), isInitialized(false) 
{
}
// Thiết lập các thông số phần cứng ban đầu

bool OLED_Display::begin() {
  Wire.begin(SDA, SCL); 

  // SỬA: Thư viện SH110X dùng begin(address, true)
  if(!display.begin(i2cAddress, true)) { 
    return false;
  }// khởi động màn hình 

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE); // SỬA: Dùng SH110X_WHITE thay vì WHITE
  display.setCursor(0, 0);
  display.display();

  isInitialized = true;
  Serial.println(F("SH1106 OLED Ready!"));

  starting();
  return true;
}

bool OLED_Display::isReady() const {
  return isInitialized;
}

void OLED_Display::clear() {// Xóa toàn bộ nội dung hiện tại
  if (isInitialized) display.clearDisplay();
}

void OLED_Display::update() {// Lệnh cực kỳ quan trọng. Mọi thứ bạn vẽ chỉ nằm trong bộ nhớ đệm, phải gọi hàm này thì hình ảnh mới thực sự hiện lên tấm kính OLED
  if (isInitialized) display.display();
}

void OLED_Display::printText(const String &text, int x, int y, uint8_t size) {
  if (!isInitialized) return;
  display.setTextSize(size);
  display.setCursor(x, y);// Đưa chữ lên màn hình tại tọa độ (x, y)
  display.print(text);
}

void OLED_Display::printTextCon(const char *text, uint8_t size) {
  if (!isInitialized) return;// Nếu màn hình chưa sẵn sàng thì không làm gì cả
  display.setTextSize(size);// Thiết lập kích thước chữ (1, 2, 3...)
  display.print(text);// Ghi chữ vào bộ đệm
  update();// Đẩy dữ liệu từ bộ đệm lên màn hình thật ngay lập tức
}

void OLED_Display::printScrollText(const String &text, int x, int y, uint8_t size, bool rightToLeft) {
  if (!isInitialized) return;
  clear();
  display.setTextSize(size);
  display.setCursor(x, y);
  display.print(text);
  update();
  // Lưu ý: Thư viện SH110X không hỗ trợ hàm scroll phần cứng, 
  // nên tạm thời tui tắt để code biên dịch được.
}

void OLED_Display::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  if (isInitialized) {
    display.drawLine(x0, y0, x1, y1, SH110X_WHITE); // SỬA: WHITE -> SH110X_WHITE
  }
}

void OLED_Display::drawScrollLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  if (!isInitialized) return;
  clear();
  for (int i = x0; i <= x1; i += 6) {
    display.drawLine(x0, y0, i, y1, SH110X_WHITE); // SỬA: WHITE -> SH110X_WHITE
    update();
    delay(2);
  }
}

void OLED_Display::starting() {
  for (int i = 0; i < 3; i++) {
    clear();
    printText("System Starting", 15, 20, 1);
    printText("...", 75, 20, 1);
    update();
    delay(800);
  }
  clear();
  printText("System Starting", 20, 20, 1);
  printText("Successfully!", 20, 35, 1);// 20 là tọa độ x( 120) 30 là tọa độ y(64) 1 là kích thước của chữ
  update();
  delay(1500);
}

void OLED_Display::Template() {
  clear();
  printText("System State:", 0, 0, 1);
  drawLine(0, 10, 127, 10);
  printText("Temp:", 0, 15, 1);
  printText("Humi:", 0, 25, 1);  
  printText("Gas: ", 0, 35, 1);
  printText("Fire:", 0, 45, 1);
  printText("Date: ", 0, 55, 1);
}

void OLED_Display::initSys() {
  clear();
  printText("Sys State:", 0, 0, 1);
  drawScrollLine(0, 10, 127, 10);
  printText("Temp:", 0, 15, 1);
  printText("Humi:", 0, 25, 1);
  printText("Gas: ", 0, 35, 1);
  printText("Fire:", 0, 45, 1);
  printText("Date: ", 0, 55, 1);
  update();
}

void OLED_Display::connectWifi() {
    if (!isInitialized) return;
    const int x = 128 - 32 - 10;
    const int y = 16;

    const uint8_t* icons[4] = {
        wifi_Dot_bmp,
        wifi_Bad_bmp,
        wifi_Good_bmp,
        wifi_Strong_bmp
    };

    display.fillRect(x, y, 32, 32, SH110X_BLACK); // SỬA: BLACK -> SH110X_BLACK
    for (uint8_t i = 0; i <= wifiState; i++) {
        display.drawBitmap(x, y, icons[i], 32, 32, SH110X_WHITE); // SỬA: WHITE -> SH110X_WHITE
    }
    update();
    wifiState = (wifiState + 1) % 4;
}

void OLED_Display::displayStatus(const String &signalQuality) {
  if (!isInitialized) return;
  const int x = 128 - 32 - 10;
  const int y = 16;

  display.fillRect(x, y, 32, 32, SH110X_BLACK); // SỬA: BLACK -> SH110X_BLACK
  if (signalQuality == "Strong")
    display.drawBitmap(x, y, wifi_Strong_bmp, 32, 32, SH110X_WHITE);// hiện chất lượn wifi
  else if (signalQuality == "Good")
    display.drawBitmap(x, y, wifi_Good_bmp, 32, 32, SH110X_WHITE);
  else
    display.drawBitmap(x, y, wifi_Bad_bmp, 32, 32, SH110X_WHITE);
}
// hiển thị System Starting sau 2,5s hiện thị Sys State:và các thông số của càm biển và độ mạnh wifi