#include "Relay.h"

Relay::Relay(uint8_t buzzgpio, uint8_t fangpio)
  : buzzerPin(buzzgpio), fanPin(fangpio) {}
  //Gán số chân GPIO của ESP32 cho hai thiết bị
  //buzzgpio (chân còi) và fangpio (chân quạt) có vẻ bạn đang định nối chúng vào chân 27 và 14.
// 27 14
bool Relay::begin() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(fanPin, LOW);
  return true;
}
void Relay::setFan(bool on) { digitalWrite(fanPin, on ? HIGH : LOW); }
//Nếu truyền vào true: Xuất mức cao (HIGH) để bật.
//Nếu truyền vào false: Xuất mức thấp (LOW) để tắt
void Relay::setBuzzer(bool on) { digitalWrite(buzzerPin, on ? HIGH : LOW); }

void Relay::isOn() {// bật còi và quạt
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(fanPin, HIGH);
}

void Relay::isOff() {// tắt còi và quạt
  digitalWrite(buzzerPin, LOW);
  digitalWrite(fanPin, LOW);
}