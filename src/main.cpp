#include "User_Config.h"

void setup() {
  Serial.begin(115200);
  initBLE();
}

void loop() {
  if (needConnect) {
    needConnect = false;
    connectWiFi(ssid_buffer, password_buffer);
  }
}
