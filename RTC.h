#ifndef RTC_H
#define RTC_H
//Header Guard giúp ngăn lỗi biên dịch nếu file này được gọi ở nhiều nơi.
#include <Wire.h>//Thư viện dùng để giao tiếp qua chuẩn I2C (Bus dữ liệu dùng để kết nối ESP32 với RTC).
#include "RTClib.h"//Thư viện chuyên dụng của Adafruit để điều khiển các dòng chip RTC như DS3231.

class RTC {
private:
    uint8_t SDA, SCL;//Lưu trữ số chân của ESP32 dùng để truyền dữ liệu (SDA) và nhịp xung (SCL).
    bool isInitialized;//Một biến cờ (flag). Nếu nó là true, nghĩa là module RTC đã được kết nối và hoạt động tốt.
    RTC_DS3231 rtc;//Đây là đối tượng đại diện cho con chip DS3231 thực tế. Nó chứa các lệnh cấp thấp để đọc thanh ghi thời gian bên trong chip.
//Địa chỉ của con này nó nằ sẵn trong thư viện rồi #include "RTClib.h" sử dụng dối tượng   RTC_DS3231 rtc
public:
    RTC(uint8_t sda = 21, uint8_t scl = 22);

    bool begin();

    // Lấy thời gian hiện tại
    DateTime now();

    // In thời gian nhiệt độ
    void print_current_time();
};

#endif
