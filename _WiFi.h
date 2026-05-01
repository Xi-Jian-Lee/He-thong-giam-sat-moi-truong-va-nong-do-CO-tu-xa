#ifndef __WIFI_H__
#define __WIFI_H__

#include <Arduino.h>

#define WIFI_CONNECT_TIMEOUT 20  // số lần thử kết nối (1 giây mỗi lần) 20s

class _WiFi {
private:
    String ssid;// tên wifi
    String password;// mật khẩu wifi

public:
    _WiFi(const String& ssid, const String& password);

    bool connect();//Lệnh yêu cầu ESP32 bắt đầu đăng nhập vào mạng. Trả về true nếu thành công.
    void disconnect();//Lệnh ngắt kết nối Wi-Fi khi không cần dùng đến.
    int signalStrength();  // trả về % tín hiệu (0–100)
    bool isConnected();//Trả về một con số từ 0 đến 100. Đây chính là hàm sử dụng công thức 2 * (dBm + 100)
    String signalQuality();  // Trả về mô tả mức tín hiệu

};

#endif
