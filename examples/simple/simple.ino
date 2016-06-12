/*
 * Simple example for LedHead library
 */
#include "LedHead.h"
#include <vector>

#define SEC_1               1000
#define SEC_3               3000
#define SEC_10              10000
#define DELAY_MILLIS        SEC_1


LedHead head = LedHead();
std::vector<uint32_t> colors = {head.yellow(), head.blue(), head.violet(), head.off()};
int curColor = 0;

void log(String message){
    if (Serial) {
        Serial.println(message);
    }
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

void setup() {
  initSerial();
}

void loop() {
  log("loop");
  
  head.updateStatusLed(head.green());
  rotateEyeColor();

  delay(SEC_1);
  head.removeStatusColor();
  delay(DELAY_MILLIS);
}
