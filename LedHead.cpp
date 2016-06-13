/*
 * LED Head - LedHead.cpp
 * (c) ringbuchblock
 */
#include "LedHead.h"
#include <vector>

#define LEDHEAD_DEBUG               false
#define LEDHEAD_LED_PIN             14
#define LEDHEAD_LED_COUNT           3
#define LEDHEAD_ALL_COLOR_DELAY     1000 // 1 sec


const uint32_t LedHead::RED = Adafruit_NeoPixel::Color(150, 0, 0);
const uint32_t LedHead::ORANGE = Adafruit_NeoPixel::Color(255, 128, 0);
const uint32_t LedHead::YELLOW = Adafruit_NeoPixel::Color(255, 255, 0);
const uint32_t LedHead::GREEN = Adafruit_NeoPixel::Color(0, 150, 0);
const uint32_t LedHead::BLUE = Adafruit_NeoPixel::Color(0, 0, 150);
const uint32_t LedHead::VIOLET = Adafruit_NeoPixel::Color(100, 0, 200);
const uint32_t LedHead::OFF = Adafruit_NeoPixel::Color(0, 0, 0);

// leds
// this must be initialized here. tried initializing it within constructor
// but got nasty exceptions all the time.
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LEDHEAD_LED_COUNT, LEDHEAD_LED_PIN, NEO_GRB + NEO_KHZ800);
uint16_t _leftEye;
uint16_t _rightEye;
uint16_t _statusLed;

// status members
bool _statusLedOff = false;
bool _eyeLedsOff = false;
uint32_t _lastEyeColor = LedHead::OFF;


// ##################################################################################
// PRIVATE
// ##################################################################################

void hLog(String message){
  if (Serial) {
    Serial.println(message);
  }
}

void hDebugLog(String message){
  if (LEDHEAD_DEBUG) {
    hLog(message);
  }
}

void LedHead::showAllColors() {
  hDebugLog("show all colors");

  std::vector<uint32_t> predefinedColors = {RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET};
  int colorCnt = predefinedColors.size();
  for(int i=0;i<colorCnt;i++) { 
    uint32_t color = predefinedColors[i]; 
    updateEyeColor(color);
    updateStatusColor(color);
    delay(LEDHEAD_ALL_COLOR_DELAY);
  }
  removeStatusColor();
  removeEyeColor();
  delay(LEDHEAD_ALL_COLOR_DELAY);
}

// ##################################################################################
// PUBLIC
// ##################################################################################

LedHead::LedHead(uint16_t l, uint16_t r, uint16_t s, uint8_t b) {
  hLog("init LED head");

  // leds
  _leftEye = l;
  _rightEye = r;
  _statusLed = s;

  // init leds
  hDebugLog("init leds");
  leds.begin(); // This initializes the NeoPixel library. 
  leds.setBrightness(b);
  leds.show();

  showAllColors();

  hDebugLog("LED head initialized");
}

LedHead::~LedHead() {
  hLog("LED head destroyed");
}

void LedHead::updateEyeColor(uint32_t color) {
  _lastEyeColor = color;
  if (_eyeLedsOff) {
    hDebugLog("ignore eye color change");
    return;
  }
  hDebugLog("update eye color");
  leds.setPixelColor(_leftEye, color);
  leds.setPixelColor(_rightEye, color);
  leds.show();
}

void LedHead::removeEyeColor() {
  updateEyeColor(OFF);
}

void LedHead::deactivateEyeLeds() {
  hDebugLog("deactivate eye leds");
  if (_eyeLedsOff) {
    hDebugLog("ignore eye leds deactivation as already inactive");
    return; // already inactive
  }
  uint32_t curColor = _lastEyeColor;
  removeEyeColor();
  _lastEyeColor = curColor;
  _eyeLedsOff = true;
}

void LedHead::activateEyeLeds() {
  hDebugLog("activate eye leds");
  if (!_eyeLedsOff) {
    hDebugLog("ignore eye leds activation as already active");
    return; //already active
  }
  _eyeLedsOff = false;
  updateEyeColor(_lastEyeColor);
}

void LedHead::updateStatusColor(uint32_t color) {
  if (_statusLedOff) {
    hDebugLog("ignore status color change");
    return;
  }
  hDebugLog("update status color");
  leds.setPixelColor(_statusLed, color);
  leds.show();
}

void LedHead::removeStatusColor() {
  updateStatusColor(OFF);
}

void LedHead::deactivateStatusLed() {
  hDebugLog("deactivate status leds");
  if (_statusLedOff) {
    hDebugLog("ignore status led deactivation as already inactive");
    return; // already inactive
  }
  removeStatusColor();
  _statusLedOff = true;
}

void LedHead::activateStatusLed() {
  hDebugLog("activate status leds");
  _statusLedOff = false;
}

