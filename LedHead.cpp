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


const uint32_t LedHead::red = Adafruit_NeoPixel::Color(150, 0, 0);
const uint32_t LedHead::orange = Adafruit_NeoPixel::Color(255, 128, 0);
const uint32_t LedHead::yellow = Adafruit_NeoPixel::Color(255, 255, 0);
const uint32_t LedHead::green = Adafruit_NeoPixel::Color(0, 150, 0);
const uint32_t LedHead::blue = Adafruit_NeoPixel::Color(0, 0, 150);
const uint32_t LedHead::violet = Adafruit_NeoPixel::Color(100, 0, 200);
const uint32_t LedHead::off = Adafruit_NeoPixel::Color(0, 0, 0);

// leds
// this must be initialized here. tried initializing it within constructor
// but got nasty exceptions all the time.
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LEDHEAD_LED_COUNT, LEDHEAD_LED_PIN, NEO_GRB + NEO_KHZ800);
uint16_t leftEye;
uint16_t rightEye;
uint16_t statusLed;

// status members
bool statusLedOff = false;
bool eyeLedsOff = false;
uint32_t lastEyeColor = LedHead::off;


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

  std::vector<uint32_t> predefinedColors = {red, orange, yellow, green, blue, violet};
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
  leftEye = l;
  rightEye = r;
  statusLed = s;

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
  lastEyeColor = color;
  if (eyeLedsOff) {
    hDebugLog("ignore eye color change");
    return;
  }
  hDebugLog("update eye color");
  leds.setPixelColor(leftEye, color);
  leds.setPixelColor(rightEye, color);
  leds.show();
}

void LedHead::removeEyeColor() {
  updateEyeColor(off);
}

void LedHead::deactivateEyeLeds() {
  hDebugLog("deactivate eye leds");
  if (eyeLedsOff) {
    hDebugLog("ignore eye leds deactivation as already inactive");
    return; // already inactive
  }
  uint32_t curColor = lastEyeColor;
  removeEyeColor();
  lastEyeColor = curColor;
  eyeLedsOff = true;
}

void LedHead::activateEyeLeds() {
  hDebugLog("activate eye leds");
  if (!eyeLedsOff) {
    hDebugLog("ignore eye leds activation as already active");
    return; //already active
  }
  eyeLedsOff = false;
  updateEyeColor(lastEyeColor);
}

void LedHead::updateStatusColor(uint32_t color) {
  if (statusLedOff) {
    hDebugLog("ignore status color change");
    return;
  }
  hDebugLog("update status color");
  leds.setPixelColor(statusLed, color);
  leds.show();
}

void LedHead::removeStatusColor() {
  updateStatusColor(off);
}

void LedHead::deactivateStatusLed() {
  hDebugLog("deactivate status leds");
  if (statusLedOff) {
    hDebugLog("ignore status led deactivation as already inactive");
    return; // already inactive
  }
  removeStatusColor();
  statusLedOff = true;
}

void LedHead::activateStatusLed() {
  hDebugLog("activate status leds");
  statusLedOff = false;
}

