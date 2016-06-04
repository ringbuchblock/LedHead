/*
 * Simple example for LedHead library using an ESP 8266.
 * 
 * 
 * Notes:
 *    WIFI:
 *        After you start the ESP, it will try to connect to WiFi. If it fails it starts in Access Point mode. 
 *        While in AP mode, connect to it then open a browser to the gateway IP, default 192.168.4.1, configure wifi, 
 *        save and it should reboot and connect.
 * 
 * 
 */

#include "LedHead.h"
#include "ESP8266WiFi.h"
#include "WiFiManager.h" // see https://github.com/tzapu/WiFiManager
#include <vector>

#define SEC_1               1000
#define SEC_3               3000
#define SEC_10              10000
#define DELAY_MILLIS        SEC_1

#define WIFI_CONFIG_AP_NAME        "esp"
#define WIFI_CONFIG_AP_PW          "espconfig"


WiFiManager wifiManager;
LedHead head;

std::vector<uint32_t> colors;
int curColor;

void log(String message){
    if (Serial) {
        Serial.println(message);
    }
}

bool wifiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool noWifi() {
  return !wifiConnected();
}

void rotateEyeColor() {
  head.updateEyeColor(colors[curColor]);
    
  curColor = curColor + 1;
  if (curColor >= colors.size()) {
    curColor = 0;
  }  
}

void initSerial() {
  Serial.begin(115200);
  Serial.println("setup");
}

void initHead() {
  head = LedHead();
  colors = {head.yellow(), head.blue(), head.violet(), head.off()};
  curColor = 0;
}

void wifiConfigModeCallback(WiFiManager *myWiFiManager) {
  log("Entered config mode");
  //log(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  //log(myWiFiManager->getConfigPortalSSID());
}

void initWifi() {
  log("init wifi");
  wifiManager.setConfigPortalTimeout(60); // 1 minute - 60 seconds
  wifiManager.setAPCallback(wifiConfigModeCallback);
  wifiManager.autoConnect(WIFI_CONFIG_AP_NAME, WIFI_CONFIG_AP_PW);
}

void setup() {
  initSerial();
  initHead();
  initWifi();
}

void loop() {
  if (noWifi()) {
    log("no wifi");
    head.updateStatusLed(head.red());
  } else {
    head.updateStatusLed(head.green());
  }

  rotateEyeColor();

  delay(SEC_1);
  // head.removeStatusColor();
  head.updateStatusLed(head.off());
  delay(DELAY_MILLIS);
}
