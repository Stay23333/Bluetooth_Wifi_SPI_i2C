#ifndef _BLE_H_
#define _BLE_H_

#include "User_Config.h"

// UUID
#define SERVICE_UUID           "e7add780-b042-4876-aae1-112855353cc1"
#define CHARACTERISTIC_UUID_RX "e7add781-b042-4876-aae1-112855353cc1"
#define CHARACTERISTIC_UUID_TX "e7add782-b042-4876-aae1-112855353cc1"

// 全局变量
extern BLECharacteristic *pTxCharacteristic;
extern String bleBuffer;
extern String ssid_buffer;
extern String password_buffer;
extern bool needConnect;

// 函数声明
void initBLE();
void sendBLEMessage(String msg);
bool parseJSON(String json);
void requestWiFiConnect();
void connectWiFi(String ssid, String password);

#endif
