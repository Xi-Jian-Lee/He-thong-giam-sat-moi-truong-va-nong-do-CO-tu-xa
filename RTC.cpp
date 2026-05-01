#include "RTC.h"
#include <Arduino.h>
// Thời gian thực (Real-Time Clock), thường là chip DS3231
RTC::RTC(uint8_t sda, uint8_t scl)
    : SDA(sda), SCL(scl), isInitialized(false) {}
//Thiết lập các chân giao tiếp I2C (SDA và SCL) mà bạn dùng để nối module RTC với ESP32.
//Biến isInitialized: Được đặt là false để báo rằng module này chưa sẵn sàng hoạt động ngay lúc mới bật nguồn.
bool RTC::begin() {
    Wire.begin(SDA, SCL);
    isInitialized = rtc.begin();

    if (isInitialized) {
        Serial.println("RTC ready");
       // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));// sau khi cập nhật lại thì nó sẽ ép cập nhật lại tg khi mới chạy
        //khi muốn cấp nhật lại tg thì ta chú thích dòng 14 nạp 1 lần sau đó ta bỏ chú thích dòng 14 nạp thêm 1 lần nữa 
    } else {
        Serial.println("RTC failed");
        return false;
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, setting time to compile time...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
//Nếu mất điện, nó sẽ tự động lấy thời gian lúc bạn biên dịch code trên máy tính để nạp vào module. Điều này giúp đồng hồ không bị chạy sai về năm 2000.
    Serial.println("RTC initialized.");
    return true;
}

DateTime RTC::now() {
    if (!isInitialized) {
        return DateTime(2000, 1, 1, 0, 0, 0);// Trả về ngày giả nếu lỗi
    }
    return rtc.now();
}
//Nó sử dụng hàm snprintf để tạo ra một chuỗi văn bản đẹp mắt
void RTC::print_current_time() {
    if (!isInitialized) {
        Serial.println("RTC not initialized.");
        return;
    }

    DateTime t = rtc.now();
    const char* daysOfWeek[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%s, %04d-%02d-%02d %02d:%02d:%02d",
             daysOfWeek[t.dayOfTheWeek()],
             t.year(), t.month(), t.day(),
             t.hour(), t.minute(), t.second());
    Serial.println(buffer);

    Serial.print("Temperature: ");
    Serial.print(rtc.getTemperature());//Chip DS3231 có một cảm biến nhiệt độ bên trong để bù trừ sai số thạch anh. Code của bạn tận dụng luôn tính năng này để in ra nhiệt độ của chính module RTC.
    Serial.println(" ºC");
}
//Thời gian thực kèm kiểm tra nhiệt độ trong broad mạch