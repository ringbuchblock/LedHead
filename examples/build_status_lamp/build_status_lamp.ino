/*
 * Simple example for LedHead library using an ESP 8266.
 */

#include "LedHead.h"        // https://github.com/ringbuchblock/LedHead
#include "ESPThing.h"       // https://github.com/ringbuchblock/ESPThing
#include "ESP8266WiFi.h"

#define SEC_1               1000
#define DELAY_MILLIS        SEC_1

// build topic and possible messages
#define TOPIC_BUILD         "build-status"
#define MSG_BUILD_SUCCESS   "OK"
#define MSG_BUILD_FAILURE   "NOT_OK"

// config topic and possible messages
#define TOPIC_CONFIG        "config"
#define MSG_OFF             "off"
#define MSG_ON              "on"
#define MSG_STATUS_OFF      "statusOff"
#define MSG_STATUS_ON       "statusOn"
#define MSG_EYES_OFF        "eyesOff"
#define MSG_EYES_ON         "eyesOn"
#define MSG_ERRORS_ONLY     "statusErrorsOnly"
#define MSG_ALL             "statusAll"

// status members
bool newBuildStatus = false;
bool error = false;
bool errorsOnly = false;
uint32_t unknown;


LedHead head;
ESPThing Thing;

void initSerial() {
  Serial.begin(115200);
  Serial.println("setup");
}

/*
void log(String message){
    if (Serial) {
        Serial.println(message);
    }
}
*/

void buildStatusCallback(const MQTT::Publish& pub) {
  String msg = pub.payload_string();
  if (msg == MSG_BUILD_SUCCESS) {
    head.updateEyeColor(head.green());
  } else if (msg == MSG_BUILD_FAILURE) {
    head.updateEyeColor(head.red());
  } else {
    head.updateEyeColor(unknown);
  }
  newBuildStatus = true;
}

void configCallback(const MQTT::Publish& pub) {
  String msg = pub.payload_string();
  if (msg == MSG_OFF) {
    head.deactivateStatusLed();
    head.deactivateEyeLeds();
  } else if (msg == MSG_ON) {
    head.activateStatusLed();
    head.activateEyeLeds();
  } else if (msg == MSG_STATUS_OFF) {
    head.deactivateStatusLed();
  } else if (msg == MSG_STATUS_ON) {
    head.activateStatusLed();
  } else if (msg == MSG_EYES_OFF) {
    head.deactivateEyeLeds();
  } else if (msg == MSG_EYES_ON) {
    head.activateEyeLeds();
  } else if (msg == MSG_ERRORS_ONLY) {
    errorsOnly = true;
    if (!error) {
      head.removeStatusColor();
    }
  } else if (msg == MSG_ALL) {
    errorsOnly = false;
  }
}

void initHead() {
  head = LedHead();
  unknown = head.blue();
  head.updateEyeColor(unknown);
}

void initThing() {
  Thing.subscribe(TOPIC_BUILD, buildStatusCallback);
  Thing.thingSubscribe(TOPIC_CONFIG, configCallback);
}

void setup() {
  initSerial();
  initHead();
  initThing();
}

bool noWifi() {
  return !Thing.wifiConnected();
}

bool noMqttConnection() {
  return !Thing.mqttClientConnected();
}

bool noThingMqttConnection() {
  return !Thing.mqttThingClientConnected();
}

void checkForErrors() {
  if (noWifi()) {
    error = true;
    head.updateStatusLed(head.red());
  } else if (noMqttConnection()) {
    error = true;
    head.updateStatusLed(head.orange());
  } else if (noThingMqttConnection()) {
    error = true;
    head.updateStatusLed(head.violet());
  } else if (!errorsOnly) {
    if (newBuildStatus) {
      head.updateStatusLed(head.green());
    } else {
      head.updateStatusLed(head.blue());
    }
  }
}

void reset() {
  head.removeStatusColor();
  newBuildStatus = false;
  error = false;
  delay(SEC_1);
}

void loop() {
  reset();
  checkForErrors();
  Thing.loop();
  delay(DELAY_MILLIS);
}

