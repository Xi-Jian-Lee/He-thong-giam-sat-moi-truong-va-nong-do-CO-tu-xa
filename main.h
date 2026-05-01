#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

#include "SHT31_Sensor.h"
#include "OLED_Display.h"
#include "Gas_Sensor.h"
#include "Fire_Sensor.h"
#include "_WiFi.h"
#include "Relay.h"
#include "App.h"
#include "RTC.h"

class SystemMain
{
private:
    // Các đối tượng cảm biến
    Fire_Sensor fireSensor;
    Gas_Sensor gasSensor;
    SHT31_Sensor shtSensor;
    OLED_Display oledDisplay;
    Relay relay;
    _WiFi _wifi;
    App app;
    RTC rtc;

    // Task handle
    TaskHandle_t Task_Fire_Sensor;
    TaskHandle_t Task_Gas_Sensor;
    TaskHandle_t Task_SHT31_Sensor;
    TaskHandle_t Task_OLED_Display;
    TaskHandle_t Task_Relay;
    TaskHandle_t Task_WiFi;
    TaskHandle_t Task_App;
    TaskHandle_t Task_RTC;

    // Dữ liệu chia sẻ giữa các task
    float temperature;
    float humidity;
    bool fireDetected;
    bool gasDetected;
    bool appMode;
    int gasValue;
    bool wifiConnected;
    int wifiStrength;
    int wifiQuality;

public:
    SystemMain();
    bool begin();
    void wifiStatus();

private:
    // Task function (static để truyền vào FreeRTOS)
    static void TaskFire(void *pvParameters);
    static void TaskGas(void *pvParameters);
    static void TaskSHT(void *pvParameters);
    static void TaskOLED(void *pvParameters);
    static void TaskRelay(void *pvParameters);
    static void TaskWiFi(void *pvParameters);
    static void TaskApp(void *pvParameters);
    static void TaskRTC(void *pvParameters);
};

#endif