/*
 * LED Head - LedHead.cpp
 * (c) ringbuchblock
 */
#include "LedHead.h"
#include <vector>

#define LEDHEAD_DEBUG               true
#define LEDHEAD_ALL_COLOR_DELAY     1000 // 1 sec
#define LEDHEAD_LED_COUNT           3



uint32_t red = Adafruit_NeoPixel::Color(150, 0, 0);


// status members
bool statusLedOff = false;
bool eyeLedsOff = false;
uint32_t lastEyeColor = 0;


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
  
  
  std::vector<uint32_t> predefinedColors = {red()}; //, orange(), colorYellow, colorGreen, colorBlue, colorViolet};

  
  int colorCnt = predefinedColors.size();
  for(int i=0;i<colorCnt;i++) { 
    hDebugLog("color: ");
    uint32_t color = predefinedColors[i]; 
    updateEyeColor(color);
    updateStatusLed(color);
    delay(LEDHEAD_ALL_COLOR_DELAY);
  }
  removeStatusColor();
  removeEyeColor();
  delay(LEDHEAD_ALL_COLOR_DELAY);
}

// ##################################################################################
// PUBLIC
// ##################################################################################

LedHead::LedHead(uint16_t left, uint16_t right, uint16_t status, uint8_t brightness, uint8_t pin, neoPixelType type) {
  hLog("init LED head");

  // leds
  leftEye = left;
  rightEye = right;
  statusLed = status;

  // init leds
  hDebugLog("init leds");
  leds = Adafruit_NeoPixel(LEDHEAD_LED_COUNT, pin, type);
  leds.begin(); // This initializes the NeoPixel library. 
  leds.setBrightness(brightness);
  leds.show(); // initializes all leds to 'off'

  // init colors
  hDebugLog("init colors");
  showAllColors();
  
  
  hDebugLog("... init finished");
}

LedHead::~LedHead() {}

uint32_t LedHead::color(uint8_t r, uint8_t g, uint8_t b) {
  return leds.Color(r,g,b);
}

uint32_t LedHead::red() {
  return color(150,0,0);
}

uint32_t LedHead::orange() {
  return color(255,128,0);
}

uint32_t LedHead::yellow() {
  return color(255,255,0);
}

uint32_t LedHead::green() {
  return color(0,150,0);
}

uint32_t LedHead::blue() {
  return color(0,0,150);
}

uint32_t LedHead::violet() {
  return color(100,0,200);
}

uint32_t LedHead::off() {
  return color(0,0,0);
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
  updateStatusLed(off());
}

void LedHead::deactivateEyeLeds() {
  hDebugLog("deactivate eye leds");
  uint32_t curColor = lastEyeColor;
  updateEyeColor(off());
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
  updateStatusLed(off());
}

void LedHead::deactivateStatusLed() {
  hDebugLog("deactivate status leds");
  updateStatusLed(off());
  statusLedOff = true;
}

void LedHead::activateStatusLed() {
  hDebugLog("activate status leds");
  statusLedOff = false;
}

