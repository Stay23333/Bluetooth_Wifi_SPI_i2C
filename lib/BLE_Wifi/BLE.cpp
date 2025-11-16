#include "User_Config.h"

// 全局变量
BLECharacteristic *pTxCharacteristic;
String bleBuffer = "";
String ssid_buffer = "";
String password_buffer = "";
bool needConnect = false;

// BLE 回调
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    String rx = String(pCharacteristic->getValue().c_str());
    Serial.print("接收碎片：");
    Serial.println(rx);

    if (!rx.startsWith("{") && !rx.endsWith("}")) {
      Serial.println("忽略无效碎片");
      return;
    }

    bleBuffer += rx;

    int start = bleBuffer.indexOf("{");
    int end   = bleBuffer.lastIndexOf("}");

    if (start >= 0 && end > start) {
      String json = bleBuffer.substring(start, end + 1);
      Serial.print("完整 JSON：");
      Serial.println(json);
      bleBuffer = "";

      if (parseJSON(json)) {
        Serial.println("JSON 解析成功！");
        requestWiFiConnect();
      } else {
        Serial.println("JSON 解析失败！");
      }
    }
  }
};

// 启动 BLE
void initBLE() {
  esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

  BLEDevice::init("ESP32S3_WIFI_CONFIG");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pRxCharacteristic =
      pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE
      );
  pRxCharacteristic->setCallbacks(new MyCallbacks());

  pTxCharacteristic =
      pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
      );
  pTxCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("蓝牙已启动，等待手机发送 WiFi 配置...");
}

// 发送 BLE 消息
void sendBLEMessage(String msg) {
  pTxCharacteristic->setValue(msg.c_str());
  pTxCharacteristic->notify();
}

// JSON 解析
bool parseJSON(String json) {
  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, json);
  if (err) return false;

  ssid_buffer     = doc["ssid"].as<String>();
  password_buffer = doc["password"].as<String>();

  Serial.printf("SSID=%s  PASS=%s\n", ssid_buffer.c_str(), password_buffer.c_str());
  return true;
}

// 标记 WiFi 连接请求
void requestWiFiConnect() {
  needConnect = true;
}
