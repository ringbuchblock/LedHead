/*
 * LED Head - LedHead.h
 * (c) ringbuchblock
 * 
 * A simple small library for controlling a total of 3 LEDs 
 * (2 for the eyes and for the status) via Adafruit's Neopixel library. 
 */
#ifndef LEDHEAD_H_
#define LEDHEAD_H_

#include "Adafruit_NeoPixel.h" //https://github.com/adafruit/Adafruit_NeoPixel
#ifdef __AVR__
  #include <avr/power.h>
#endif


class LedHead {

  public:
      LedHead(uint16_t leftEye=2, uint16_t rightEye=1, uint16_t statusLed=0, uint8_t brightness=30);
      ~LedHead();

      // pre-defined colors:
      // you can define more colors by using Adafruit_NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b))
      const static uint32_t 
        RED, 
        ORANGE,
        YELLOW,
        GREEN,
        BLUE,
        VIOLET,
        OFF;

      // interfaces for eye color control
      void updateEyeColor(uint32_t color);
      void removeEyeColor();
      void deactivateEyeLeds();
      void activateEyeLeds();

      // interfaces for status LED color control
      void updateStatusColor(uint32_t color);
      void removeStatusColor();
      void deactivateStatusLed();
      void activateStatusLed();

  private:
      void showAllColors();
};


#endif /* LEDHEAD_H_ */
