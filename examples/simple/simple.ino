/*
 * Simple example for LedHead library
 */
#include "LedHead.h"
#include <vector>

#define SEC_1               1000

LedHead head;
int curColor = 0;

void log(String message){
  if (Serial) {
    Serial.println(message);
  }
}

void rotateEyeColor() {
  std::vector<uint32_t> colors = {LedHead::yellow, LedHead::blue, LedHead::violet};
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
}

void setup() {
  initSerial();
  initHead();
}

void loop() {
  log("loop");
  
  head.updateStatusColor(LedHead::green);
  rotateEyeColor();

  delay(SEC_1);
  head.removeStatusColor();
  delay(SEC_1);
}
