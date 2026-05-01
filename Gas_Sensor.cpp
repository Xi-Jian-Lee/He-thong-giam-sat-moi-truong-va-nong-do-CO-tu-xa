#include "esp32-hal-adc.h"
#include "esp32-hal-gpio.h"
#include "Gas_Sensor.h"

Gas_Sensor::Gas_Sensor(uint8_t gpio, int threshold)//Tham số gpio: Chân cắm trên ESP32 (phải là chân có chức năng ADC - chuyển đổi tương tự sang số).
    : pin(gpio), thresholdLevel(threshold) {}//Tham số threshold: Ngưỡng nồng độ (ví dụ: 1000). Nếu nồng độ gas vượt quá con số này, hệ thống sẽ coi là có rò rỉ khí hoặc có khói.

bool Gas_Sensor::begin()
{
    pinMode(pin, INPUT_PULLUP);
    return true;
}

int Gas_Sensor::readRaw()
{// Đây là lệnh quan trọng nhất. Nó đọc điện áp từ cảm biến (từ 0V đến 3.3V) và chuyển thành một con số trong khoảng 0 đến 4095 
    return analogRead(pin);
}

bool Gas_Sensor::isGasDetected()
{//Hàm này lấy giá trị nồng độ hiện tại (value) đem so sánh với mức ngưỡng (thresholdLevel) mà bạn đã cài đặt ở hàm khởi tạo.
    int value = readRaw();
    return (value > thresholdLevel);
    // return readRaw();
}
   