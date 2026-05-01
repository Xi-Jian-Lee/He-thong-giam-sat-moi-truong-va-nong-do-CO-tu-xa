#ifndef FIRE_SENSOR_H
#define FIRE_SENSOR_H

#include <Arduino.h>

class Fire_Sensor
{
private:
    uint8_t pin;

public:
    Fire_Sensor(uint8_t gpio);
// Hàm khởi tạo. Khi bạn tạo ra một thực thể cảm biến (ví dụ: Fire_Sensor myFireSensor(15);), nó sẽ gán số chân 15 vào biến pin ở trên.
    bool begin();// Hàm thiết lập ban đầu. Nó trả về kiểu bool (đúng/sai) để báo cho hệ thống biết việc cấu hình chân tín hiệu đã sẵn sàng chưa
    bool read();// Hàm đọc tín hiệu thô từ cảm biến. Nó sẽ trả về trạng thái điện tử (0 hoặc 1) sau khi đã xử lý logic.
    bool isFire();// Đây là hàm quan trọng nhất cho người dùng. Nó trả về true nếu có lửa và false nếu an toàn.
};

#endif
