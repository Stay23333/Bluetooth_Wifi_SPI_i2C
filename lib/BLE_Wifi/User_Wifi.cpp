#include "User_Config.h"

void connectWiFi(String ssid, String password) {
  Serial.println("准备连接 WiFi...");

  WiFi.disconnect(true);
  delay(200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi 连接成功！");
    sendBLEMessage("{\"status\":\"ok\",\"ip\":\"" + WiFi.localIP().toString() + "\"}");
  } else {
    Serial.println("WiFi 连接失败！");
    sendBLEMessage("{\"status\":\"fail\"}");
  }
}
