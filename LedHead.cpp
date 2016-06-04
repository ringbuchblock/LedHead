/*
 * LED Head - LedHead.cpp
 * (c) ringbuchblock
 */
#include "LedHead.h"
#include <vector>

#define DEBUG               false

#define SEC_1               1000
#define SEC_10              10000
#define DELAY_MILLIS        SEC_1
#define ALL_COLOR_DELAY     SEC_1

#define LED_COUNT      3

Adafruit_NeoPixel leds;

// leds
uint16_t leftEye;
uint16_t rightEye;
uint16_t statusLed;
uint8_t brightness;

// colors
uint32_t colorRed;
uint32_t colorOrange;
uint32_t colorGreen;
uint32_t colorBlue;
uint32_t colorViolet;
uint32_t colorYellow;
uint32_t colorOff;
const std::vector<uint32_t> allColors = {colorRed, colorOrange, colorYellow, colorGreen, colorBlue, colorViolet};
const uint32_t colorUnknown = colorBlue;

// status members
bool statusLedOff = false;
bool eyeLedsOff = false;
bool error = false;
bool errorsOnly = false;
uint32_t lastEyeColor = colorUnknown;


// ##################################################################################
// PRIVATE
// ##################################################################################

void hLog(String message){
  if (Serial) {
    Serial.println(message);
  }
}

void hDebugLog(String message){
  if (DEBUG) {
    hLog(message);
  }
}

void LedHead::showAllColors() {
  int colorCnt = allColors.size();
  for(int i=0;i<colorCnt;i++) { 
    uint32_t color = allColors[i]; 
    updateEyeColor(color);
    updateStatusLed(color);
    delay(ALL_COLOR_DELAY);
  }
  updateStatusLed(colorOff);
  updateEyeColor(colorOff);
  delay(ALL_COLOR_DELAY);
}

// ##################################################################################
// PUBLIC
// ##################################################################################

LedHead::LedHead(uint16_t leftEye, uint16_t rightEye, uint16_t statusLed, uint8_t brightness, uint8_t pin, neoPixelType type) {
  hLog("init LED head");

  // leds
  leftEye = leftEye;
  rightEye = rightEye;
  statusLed = statusLed;

  // init leds
  leds = Adafruit_NeoPixel(LED_COUNT, pin, type);
  leds.begin(); // This initializes the NeoPixel library. 
  leds.setBrightness(brightness);

  // init colors
  colorRed = leds.Color(150, 0, 0);
  colorOrange = leds.Color(255, 128, 0);
  colorGreen = leds.Color(0, 150, 0);
  colorBlue = leds.Color(0, 0, 150);
  colorViolet = leds.Color(100, 0, 200);
  colorYellow = leds.Color(255, 255, 0);
  colorOff = leds.Color(0, 0, 0);

  showAllColors();
}

LedHead::~LedHead() {}

uint32_t LedHead::color(uint8_t r, uint8_t g, uint8_t b) {
  return leds.Color(r,g,b);
}

uint32_t LedHead::red() {
  return colorRed;
}

uint32_t LedHead::orange() {
  return colorOrange;
}

uint32_t LedHead::yellow() {
  return colorYellow;
}

uint32_t LedHead::green() {
  return colorGreen;
}

uint32_t LedHead::blue() {
  return colorBlue;
}

uint32_t LedHead::violet() {
  return colorViolet;
}

uint32_t LedHead::off() {
  return colorOff;
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
  updateStatusLed(colorOff);
}

void LedHead::deactivateEyeLeds() {
  hDebugLog("deactivate eye leds");
  uint32_t curColor = lastEyeColor;
  updateEyeColor(colorOff);
  lastEyeColor = curColor;
  eyeLedsOff = true;
}

void LedHead::activateEyeLeds() {
  hDebugLog("activate eye leds");
  eyeLedsOff = false;
  updateEyeColor(lastEyeColor);
}

void LedHead::updateStatusLed(uint32_t color) {
  if (statusLedOff) {
    hDebugLog("ignore status color change");
    return;
  }
  hDebugLog("update status color");
  leds.setPixelColor(statusLed, color);
  leds.show();
}

void LedHead::removeStatusColor() {
  updateStatusLed(colorOff);
}

void LedHead::deactivateStatusLed() {
  hDebugLog("deactivate status leds");
  updateStatusLed(colorOff);
  statusLedOff = true;
}

void LedHead::activateStatusLed() {
  hDebugLog("activate status leds");
  statusLedOff = false;
}

