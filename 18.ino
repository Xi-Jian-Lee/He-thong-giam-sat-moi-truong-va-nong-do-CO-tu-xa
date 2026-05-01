// #include "_WiFi.h"

// _WiFi _wifi("Thanh An", "linhtuyen78");
#include "main.h"
SystemMain sysMain;// Khởi tạo một đối tượng tên là sysMain. Mọi logic phức tạp về cảm biến khói, lửa, nhiệt độ và Blynk mà bạn mô tả đều được "gói" gọn bên trong đối tượng này
void setup()
{
  Serial.begin(115200);
  // _wifi.connect();
  // Serial.println(_wifi.signalStrength());
  sysMain.begin();//Đây là lệnh quan trọng nhất. Nó sẽ kích hoạt các lệnh khởi tạo bên trong file main.cpp
}

void loop()
{
  // if (!_wifi.isConnected()) {
  //   Serial.println("WiFi mất kết nối – đang thử lại...");
  //   _wifi.connect();
  // }
  // Serial.printf("Tín hiệu: %d%% - %s\n",
  //               _wifi.signalStrength(),
  //               _wifi.signalQuality().c_str());
  // delay(1000);




  // tên file có đuôi .cpp là file chứa lệnh thực thi của các hàm được liệt kê trong file đuôi .h còn file đuôi .h chứa các hàm nhưng ko chứa lệnh thực thi trong đó
  // code này sẽ chạy file main.h
}
