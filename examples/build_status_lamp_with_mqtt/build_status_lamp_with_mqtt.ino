/*
 * Simple example for LedHead library using an ESP 8266 and a MQTT client library.
 * 
 * Notes:
 *    WIFI:
 *        After you start the ESP, it will try to connect to WiFi. If it fails it starts in Access Point mode. 
 *        While in AP mode, connect to it then open a browser to the gateway IP, default 192.168.4.1, configure wifi, 
 *        save and it should reboot and connect.
 * 
 */

#include <ESP8266WiFi.h>          // https://github.com/esp8266/Arduino
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>         // https://github.com/knolleary/PubSubClient
#include "LedHead.h"

#define SEC_1                   1000
#define SEC_15                  15000

#define MQTT_KEEPALIVE          60 // this is a constant of PubSubClient which we will override
#define MQTT_RECONNECT_TIMEOUT  SEC_15
#define MQTT_SERVER             "<your mqtt server>"
#define MQTT_PORT               1883

#define MQTT_BUILD_STATUS_TOPIC "<your mqtt topic>"
#define MSG_BUILD_SUCCESS       "SUCCESSFUL"
#define MSG_BUILD_FAILURE       "FAILED"
#define MSG_BUILD_TIMEOUT       "TIMEOUT"

#define WIFI_CONFIG_AP_NAME     "esp"
#define WIFI_CONFIG_AP_PW       "espconfig"


LedHead head;
WiFiClient wc;
PubSubClient mqttClient;


bool newBuildStatus = false;
int _mqtt_last_connect;



bool mqttConnected() {
  return mqttClient.connected();
}

bool noMqtt() {
  return !mqttConnected();
}

bool wifiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool noWifi() {
  return !wifiConnected();
}

void mqttSubscribeCallback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg.concat((char)payload[i]);
  }

/*
  char* msg = (char*)malloc(length);
  memcpy(msg,payload,length);
*/

  handleMqttMessage(topic, msg);
}

void handleMqttMessage(const String& topic, const String& msg) {
  Serial.print("[NEW MSG] ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);
  if (topic == MQTT_BUILD_STATUS_TOPIC) {
    handleBuildStatusMsg(msg);
  }
}

void handleBuildStatusMsg(const String& msg) {
  if (msg == MSG_BUILD_SUCCESS) {
    head.updateEyeColor(LedHead::GREEN);
  } else if (msg == MSG_BUILD_FAILURE) {
    head.updateEyeColor(LedHead::RED);
  } else if (msg == MSG_BUILD_TIMEOUT) {
    head.updateEyeColor(LedHead::YELLOW);
  } else {
    head.updateEyeColor(LedHead::VIOLET);
  }
  newBuildStatus = true;
}

void mqttReconnect() {
  int now = millis();
  if (_mqtt_last_connect + MQTT_RECONNECT_TIMEOUT > now) {
    Serial.println("skippinG mqtt reconnect as not due yet");
    return;
  }
  
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  if (mqttClient.connect("bob")) {
    Serial.println("connected");
    // Once connected, publish an announcement...
    mqttClient.publish("build-bob", "hello world");
    mqttClient.subscribe(MQTT_BUILD_STATUS_TOPIC);
  } else {
    Serial.print("failed, rc=");
    Serial.println(mqttClient.state());
  }
  _mqtt_last_connect = millis();
}

void initSerial() {
  Serial.begin(115200);
}

void initHead() {
  head = LedHead();
  head.updateEyeColor(LedHead::BLUE);
}

void initWifi() {
  Serial.println("init wifi");
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(120); // seconds
  wifiManager.autoConnect(WIFI_CONFIG_AP_NAME, WIFI_CONFIG_AP_PW);
}

void initMqtt() {
  Serial.println("init mqtt");
  mqttClient
    .setClient(wc)
    .setServer(MQTT_SERVER, MQTT_PORT)
    .setCallback(mqttSubscribeCallback);
}

void setup() {
  initSerial();
  initHead();
  initWifi();
  initMqtt();
}

void checkForErrors() {
  if (noWifi()) {
    head.updateStatusColor(LedHead::RED);
  } else if (noMqtt()) {
    head.updateStatusColor(LedHead::ORANGE);
  } else {
    if (newBuildStatus) {
      head.updateStatusColor(LedHead::GREEN);
    } else {
      head.updateStatusColor(LedHead::BLUE);
    }
  }
}

void headLoop() {
  head.removeStatusColor();
  newBuildStatus = false;
  delay(SEC_1);
  checkForErrors();
}

void loop() {
  headLoop();

  if (wifiConnected()) {
    if (!mqttConnected()) {
      mqttReconnect();
    } else {
      mqttClient.loop();
    }
  }
  delay(SEC_1);
}
