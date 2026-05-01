#include "_WiFi.h"
#include <WiFi.h>

_WiFi::_WiFi(const String& ssid, const String& password)
  : ssid(ssid), password(password) {}
  // pass vs ten wifi nam o code main.cpp dong 11
  
bool _WiFi::connect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.printf(" Đang kết nối tới WiFi: %s\n", ssid.c_str());

  uint16_t count = 0;
  while (WiFi.status() != WL_CONNECTED && count < WIFI_CONNECT_TIMEOUT) {
    delay(1000);
    Serial.print(".");
    count++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\n Kết nối thành công! IP: %s\n", WiFi.localIP().toString().c_str());
    return true;
  } else {
    Serial.println("\n Kết nối WiFi thất bại!");
    return false;
  }
}

void _WiFi::disconnect() {
  WiFi.disconnect(true);
  Serial.println(" Đã ngắt kết nối WiFi");
}

int _WiFi::signalStrength() {/// hàm đo cường độ tín hiệu 
  if (WiFi.status() != WL_CONNECTED)// trả về cường độ tín hiệu theo đơn vị dBm một số âm càng gần 0 thì càng mạnh
    return -1;

  int dBm = WiFi.RSSI();
  // Serial.printf(" RSSI: %d dBm\n", dBm);
// giá trị trả về gần =0 là càng mạnh
  if (dBm <= -100) return 0;
  if (dBm >= -50) return 100;
  return 2 * (dBm + 100);
}
// áp dụng công thức 2*(-75)+100=50%
bool _WiFi::isConnected() {
  return (WiFi.status() == WL_CONNECTED);
}
String _WiFi::signalQuality() {
  if (!isConnected()) return "Disconnect";

  int dBm = WiFi.RSSI();

  if (dBm >= -50) return "Strong";
  else if (dBm >= -65) return "Good";
  else if (dBm >= -80) return "Weak";
  else return "Rất yếu";
}