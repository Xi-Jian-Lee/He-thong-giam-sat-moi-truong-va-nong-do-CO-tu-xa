#include "main.h"

SystemMain::SystemMain()
  : fireSensor(4),// cảm biến lửa là chân 4
    gasSensor(33, 1860),// cảm biến gass là chân 33 ngưỡng bao động là 1860
    shtSensor(21, 22, 0x44),//Cảm biến nhiệt độ SHT31 dùng I2C (21, 22
    oledDisplay(21, 22, 0x3C, 128, 64, -1),//21 là chân SDA 22 là chân SCL , 0x3c địa chỉ IC2 của màn hình , 128 là chiều rộng màn hình 128pixel, 64 là chiều cao màn hình (64 pixel)
    relay(27, 14),//Còi/Quạt nối chân 27 và 14
     _wifi("HTN2", "LIENMINH"),// tên wifi và pass
   

   
    //_wifi("MrGonf", "1234567890"),// tên wifi và pass nhớ đổi khi cần wifi khác
    // nhớ đổi pass và tên trong app.h dòng 18 19

    app(),
    rtc(21, 22) {
}

bool SystemMain::begin() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  oledDisplay.begin();
  vTaskDelay(pdMS_TO_TICKS(200));
  oledDisplay.initSys();
  shtSensor.begin();
  vTaskDelay(pdMS_TO_TICKS(200));
  rtc.begin();
  vTaskDelay(pdMS_TO_TICKS(200));
  gasSensor.begin();
  fireSensor.begin();
  relay.begin();
  vTaskDelay(pdMS_TO_TICKS(200));
  _wifi.connect();
  vTaskDelay(pdMS_TO_TICKS(500));
  app.begin();
  vTaskDelay(pdMS_TO_TICKS(200));

  xTaskCreatePinnedToCore(
    TaskFire,           // Hàm xử lý chính của task
    "FireSensor",       // Tên task
    2048,               // Stack size (bytes)
    this,               // Tham số truyền vào task
    2,                  // Priority
    &Task_Fire_Sensor,  // Handle của task
    1                   // Core ID
  );
  xTaskCreatePinnedToCore(TaskGas, "GasSensor", 2048, this, 2, &Task_Gas_Sensor, 1);
  xTaskCreatePinnedToCore(TaskSHT, "SHTSensor", 3072, this, 2, &Task_SHT31_Sensor, 1);
  xTaskCreatePinnedToCore(TaskOLED, "OLED", 4096, this, 1, &Task_OLED_Display, 1);
  xTaskCreatePinnedToCore(TaskRelay, "RELAY", 1024, this, 2, &Task_Relay, 1);
  xTaskCreatePinnedToCore(TaskWiFi, "_WiFi", 4096, this, 2, &Task_WiFi, 0);
  xTaskCreatePinnedToCore(TaskApp, "APP", 4096, this, 2, &Task_App, 0);
  xTaskCreatePinnedToCore(TaskRTC, "RTC", 4096, this, 2, &Task_RTC, 1);

  return true;
}

void SystemMain::wifiStatus() {
  if (_wifi.isConnected()) {
    oledDisplay.displayStatus(_wifi.signalQuality());
  } else {
    oledDisplay.connectWifi();
  }
}

//============================== TaskRTC =======================================//
void SystemMain::TaskRTC(void *pvParameters) {
  SystemMain *sys = (SystemMain *)pvParameters;
  for (;;) {// vòng lập vô tận luôn cập nhật trạng thái 
    // sys->rtc.print_current_time();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

//============================== TaskApp =======================================//
void SystemMain::TaskApp(void *pvParameters) {
  SystemMain *sys = (SystemMain *)pvParameters;
  for (;;) {
    sys->app.update();// duy trì kết nối vs sever
    sys->app.updateSensor(sys->temperature, sys->humidity, sys->gasValue, sys->fireDetected);//Đẩy các thông số Nhiệt độ, Độ ẩm, Gas, và Lửa lên App để bạn có thể xem từ xa.
    vTaskDelay(pdMS_TO_TICKS(500));// tầ suất 0.5 s 1 lần
  }
}

//============================== TaskRelay =======================================//
void SystemMain::TaskRelay(void *pvParameters) {
  SystemMain *sys = (SystemMain *)pvParameters;
  static uint32_t manualStart = 0;
  const uint32_t MANUAL_TIMEOUT = 15UL * 1000UL;  // 5UL * 60UL * 1000UL
  for (;;) {
    bool danger = (sys->gasDetected || sys->fireDetected || sys->temperature > 42); // nếu nhiệt độ lớn hơn 42 thì báo cháy, phát hiện rò gas và lửa
    if (modeState == MODE_AUTO) {
      Serial.println("modeState == AUTO");
      if (danger) {
        sys->relay.isOn();
      } else if (!danger) {
        sys->relay.isOff();
      }
      manualStart = 0;
    } else {

      Serial.println("modeState == MANUAL");// chế độ thù công sau 15s ko tương tác sẽ tính toán lại cho an toàn
      if (manualStart == 0) {
        manualStart = millis();
      } else if (millis() - manualStart > MANUAL_TIMEOUT) {
        manualStart = 0;
        // modeState = MODE_AUTO;
        sys->app.setLastSentMode();
        Serial.println("[RELAY] Auto-revert to AUTO");
      }
    }

    // sys->wifiQuality = sys->_wifi.signalQuality();

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

//============================== TaskWiFi =======================================//
void SystemMain::TaskWiFi(void *pvParameters) {
  SystemMain *sys = (SystemMain *)pvParameters;
  for (;;) {
    sys->wifiStrength = sys->_wifi.signalStrength();// liên tục lấy giá trị từ router tính bằng dBm
    // sys->wifiQuality = sys->_wifi.signalQuality();

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

//============================== TaskFire =======================================//
void SystemMain::TaskFire(void *pvParameters) {
  SystemMain *sys = (SystemMain *)pvParameters;
  for (;;) {
    sys->fireDetected = sys->fireSensor.read();// đọc xem có lửa ko
    vTaskDelay(pdMS_TO_TICKS(500));// chạy liên tục cập nhật mổi 0,5s
  }
}
//============================== TaskGas =======================================//
void SystemMain::TaskGas(void *pvParameters) {
  SystemMain *sys = (SystemMain *)pvParameters;
  for (;;) {
    sys->gasValue = sys->gasSensor.readRaw();// đọc giá trị gas
    sys->gasDetected = sys->gasSensor.isGasDetected();// so sánh xem an toàn hay không

    // int raw = sys->gasSensor.readRaw();
    // Serial.print("Gas Value: ");
    // Serial.println(raw);
    // if (sys->gasSensor.isGasDetected())
    // {
    //     Serial.println("Gas detected!");
    // }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
//============================== TaskSHT =======================================//
void SystemMain::TaskSHT(void *pvParameters) {
  SystemMain *sys = (SystemMain *)pvParameters;
  for (;;) {
    sys->temperature = sys->shtSensor.readTemperature();
    sys->humidity = sys->shtSensor.readHumidity();
    vTaskDelay(pdMS_TO_TICKS(1000));// giám sát độ ẩm nhưng ko dùng độ ẩm để kích hoạt báo động
  }
}
//============================== TaskOLED ========================================// úp dữ liệu lên oled
void SystemMain::TaskOLED(void *pvParameters) {
  SystemMain *sys = (SystemMain *)pvParameters;

  char buffer[128];

  for (;;) {
    sys->oledDisplay.clear();// xóa sạch bộ nhớ đệm
    sys->oledDisplay.Template();// vẽ lại các nhãn tính nhiệt độm độ ẩm, gas
    // --- Nhiệt độ ---
    snprintf(buffer, sizeof(buffer), "%.1f C", sys->temperature);
    sys->oledDisplay.printText(buffer, 30, 15, 1);// ghi vào tọa độ xác định

    // --- Độ ẩm ---
    snprintf(buffer, sizeof(buffer), "%.1f %%", sys->humidity);
    sys->oledDisplay.printText(buffer, 30, 25, 1);

    // --- Gas ---
    snprintf(buffer, sizeof(buffer), "%d ppm", sys->gasValue);
    sys->oledDisplay.printText(buffer, 30, 35, 1);

    // --- Fire ---
    snprintf(buffer, sizeof(buffer), "%s", sys->fireDetected ? "YES" : "NO");
    sys->oledDisplay.printText(buffer, 30, 45, 1);



    // --- RTC thời gian ---
    DateTime t = sys->rtc.now();
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d|%02d:%02d", t.year(), t.month(), t.day(), t.hour(), t.minute());
    sys->oledDisplay.printText(buffer, 30, 55, 1);

    // --- Wifi ---
    sys->wifiStatus();
    // Cập nhật màn hình OLED
    sys->oledDisplay.update();// chốt hạ cập nhật

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

//Người sản xuất: TaskFire, TaskGas, TaskSHT đọc dữ liệu từ cảm biến rồi cất vào các biến chung (sys->temperature, sys->gasDetected,...).

//Người điều phối: TaskRelay nhìn vào các biến đó để quyết định bật hay tắt còi.

//Người báo cáo: TaskOLED và TaskApp lấy dữ liệu đó hiển thị cho người dùng xem.