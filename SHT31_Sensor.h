#ifndef SHT31_SENSOR_H
#define SHT31_SENSOR_H

#include <Wire.h>//Thư viện dùng để giao tiếp I2C giữa ESP32 và cảm biến.
#include <Adafruit_SHT31.h>//Thư viện chính từ Adafruit chứa các lệnh cấp thấp để điều khiển con chip SHT31.

class SHT31_Sensor {
private:
    uint8_t SDA, SCL, i2cAddress;//Lưu số chân cắm và địa chỉ I2C (mặc định là 0x44).
    bool isInitialized;//Một biến cờ (flag) kiểu Đúng/Sai để đánh dấu cảm biến đã khởi động thành công hay chưa.
    Adafruit_SHT31 sht31;//Đối tượng thực tế dùng để giao tiếp với phần cứng.

public:
    SHT31_Sensor(uint8_t sda = 21, uint8_t scl = 22, uint8_t address = 0x44);//Hàm khởi tạo với các giá trị mặc định cho ESP32: Chân 21 (SDA) và 22 (SCL).
    bool begin();//Lệnh "đánh thức" cảm biến
    float readTemperature();//Trả về nhiệt độ hiện tại (kiểu số thực - độ C)
    float readHumidity();//Trả về độ ẩm hiện tại (kiểu số thực - %).
    bool isReady() const;//Kiểm tra nhanh xem cảm biến có đang hoạt động ổn định hay không mà không làm thay đổi cài đặt của nó.
    void printData(Stream &out = Serial);//Một hàm tiện ích để in nhanh cả nhiệt độ và độ ẩm ra màn hình máy tính
};

#endif
// cảm biến nhiệt độ và độ ẩm Với SDA/SCL, bạn có thể nối tất cả các thiết bị này vào cùng 2 sợi dây đó. ESP32 sẽ phân biệt chúng bằng "Địa chỉ" (như số nhà).
// giao tức i2c
//Địa chỉ 0x44 là SHT31.
//Cần độ chính xác cực cao, dữ liệu phức tạp
//Địa chỉ 0x3C là OLED.
//Cần độ chính xác cực cao, dữ liệu phức tạp
//Địa chỉ 0x68 là RTC.
// các cảm biến trên có Bộ xử lý i2c

// Cảm biến lửa và gas ko sài vì lửa là digital và gas là analong chỉ cần biết có hay không và ko có hệ thống i2c