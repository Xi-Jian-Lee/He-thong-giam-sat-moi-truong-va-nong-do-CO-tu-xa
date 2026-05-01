#ifndef GAS_SENSOR_H
#define GAS_SENSOR_H

#include <Arduino.h>

class Gas_Sensor
{
private:
    uint8_t pin;// // Lưu số chân GPIO kết nối với cảm biến
    int thresholdLevel;//// Lưu ngưỡng nồng độ để kích hoạt báo động

public:
    Gas_Sensor(uint8_t gpio, int threshold = 1000);
// hàm khởi tạo chân và giá trị của cảm biến khí gas
    bool begin();
    int readRaw();//Trả về một số nguyên (0 - 4095 trên ESP32). Con số này đại diện cho nồng độ khí gas thô mà cảm biến cảm nhận được.
    bool isGasDetected();//Trả về kiểu Đúng/Sai (true/false). Hàm này sẽ lấy giá trị từ readRaw() so sánh với thresholdLevel để đưa ra kết luận cuối cùng là có phát hiện rò rỉ khí/khói hay không.
};

#endif
// 1000 là giá trị kham khảo nó giống như trong môi trường nấu ăn rất dễ báo sai nên tụi em ghi đê lên nó bằng file main.cpp là 1860