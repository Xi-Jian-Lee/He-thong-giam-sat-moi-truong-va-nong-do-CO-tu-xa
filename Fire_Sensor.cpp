#include "Fire_Sensor.h"

Fire_Sensor::Fire_Sensor(uint8_t gpio) : pin(gpio) {}//Khi bạn khai báo một cảm biến lửa mới, bạn truyền số chân (GPIO) mà bạn đã cắm cảm biến đó vào.

bool Fire_Sensor::begin()
{
    pinMode(pin, INPUT_PULLUP);//Thiết lập chân này là đầu vào (INPUT). Chế độ PULLUP sẽ kích hoạt một điện trở treo nội bộ bên trong chip ESP32
    //Giữ cho tín hiệu luôn ở mức cao (High) khi không có lửa, giúp tránh nhiễu tín hiệu điện khi dây dẫn dài hoặc môi trường có từ trường
    return true;
}

bool Fire_Sensor::read()
{
    return !digitalRead(pin);
    //Do các cảm biến lửa thông dụng trên thị trường thường trả về mức THẤP (0) khi phát hiện có lửa và mức CAO (1) khi bình thường
    // Nếu phát hiện lửa (0) trả về 1
}

bool Fire_Sensor::isFire()
{
    return read();//có lữa trả về true isFire tra ve true
}
