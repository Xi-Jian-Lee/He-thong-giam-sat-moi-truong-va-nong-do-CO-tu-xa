#include "SHT31_Sensor.h"

SHT31_Sensor::SHT31_Sensor(uint8_t sda, uint8_t scl, uint8_t address)
    : SDA(sda), SCL(scl), i2cAddress(address), isInitialized(false) {}
//Nhận vào chân dữ liệu (SDA), chân nhịp xung (SCL) và địa chỉ I2C (thường là 0x44).
//Biến isInitialized được đặt là false để đảm bảo hệ thống không đọc dữ liệu khi cảm biến chưa sẵn sàng.
bool SHT31_Sensor::begin()// Khởi động cảm biến 
{
    // Wire.begin(SDA, SCL);
    if (!sht31.begin(i2cAddress)) // I2C address = 0x44
    {
        Serial.println("SHT31 not found!");
        isInitialized = false;
        return false;
    }
    isInitialized = true;
    Serial.println("SHT31 initialized.");
    return true;
}

float SHT31_Sensor::readTemperature()//Trả về giá trị nhiệt độ (độ C) và độ ẩm (%).
{
    if (!isInitialized)
        return NAN;
    return sht31.readTemperature();
}

float SHT31_Sensor::readHumidity()//Trả về giá trị độ ẩm tương đối (%) từ cảm biến SHT31.
{
    if (!isInitialized)
        return NAN;
    return sht31.readHumidity();
}

bool SHT31_Sensor::isReady() const
{//Cho phép các bộ phận khác của chương trình (như file main.cpp) kiểm tra xem cảm biến SHT31 có đang hoạt động bình thường hay không
    return isInitialized;
}

void SHT31_Sensor::printData(Stream &out)//Xuất dữ liệu đã định dạng đẹp mắt ra Serial Monitor
{
    if (!isInitialized)
    {
        out.println("SHT31 not initialized.");
        return;
    }
    float t = readTemperature();
    float h = readHumidity();
    out.print("Temp: ");
    out.print(t, 2);
    out.print(" °C  |  Humidity: ");
    out.print(h, 2);
    out.println(" %");
}
