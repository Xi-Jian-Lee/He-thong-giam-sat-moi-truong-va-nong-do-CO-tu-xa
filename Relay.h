#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {
private:
    uint8_t buzzerPin;
    uint8_t fanPin;

public:
    Relay(uint8_t buzzgpio, uint8_t fangpio);

    bool begin();
  void setFan(bool on);// Điều khiển quạt. Truyền true để bật, false để tắt
  void setBuzzer(bool on);//Điều khiển còi. Truyền true để bật, false để tắt.
  void isOn();   // alias tổng Một cách gọi nhanh để bật cả hai thiết bị cùng lúc
  void isOff();  // alias tổngMột cách gọi nhanh để tắt cả hai thiết bị cùng lúc

};

#endif