/*
 * LED Head - LedHead.cpp
 * (c) by ringbuchblock
 */
#include "LedHead.h"
#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include <vector>

#define DEBUG               false

#define SEC_1               1000
#define SEC_10              10000
#define DELAY_MILLIS        SEC_1
#define ALL_COLOR_DELAY     SEC_1

#define DATA_PIN       14
#define LED_COUNT      3
#define LEFT_EYE       2
#define RIGHT_EYE      1
#define STATUS_LED     0
#define BRIGHTNESS     30
  
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, DATA_PIN, NEO_GRB + NEO_KHZ800);

// colors
const uint32_t colorRed = leds.Color(150, 0, 0);
const uint32_t colorOrange = leds.Color(255, 128, 0);
const uint32_t colorGreen = leds.Color(0, 150, 0);
const uint32_t colorBlue = leds.Color(0, 0, 150);
const uint32_t colorViolet = leds.Color(100, 0, 200);
const uint32_t colorYellow = leds.Color(255, 255, 0);
const std::vector<uint32_t> allColors = {colorRed, colorOrange, colorYellow, colorGreen, colorBlue, colorViolet};
const uint32_t colorOff = leds.Color(0, 0, 0);
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

LedHead::LedHead() {
  hLog("init LED head");

  leds.begin(); // This initializes the NeoPixel library. 
  leds.setBrightness(BRIGHTNESS);
  
  showAllColors();
}

LedHead::~LedHead() {
  
}

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
  leds.setPixelColor(LEFT_EYE, color);
  leds.setPixelColor(RIGHT_EYE, color);
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
  leds.setPixelColor(STATUS_LED, color);
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

