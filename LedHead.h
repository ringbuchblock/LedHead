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
      LedHead(uint16_t leftEye=2, uint16_t rightEye=1, uint16_t statusLed=0, uint8_t brightness=30, uint8_t pin=14, neoPixelType type=NEO_GRB + NEO_KHZ800);
      ~LedHead();

      // use this to define a color yourself
      uint32_t color(uint8_t r, uint8_t g, uint8_t b);

      // pre-defined colors
      uint32_t red();
      uint32_t orange();
      uint32_t yellow();
      uint32_t green();
      uint32_t blue();
      uint32_t violet();
      uint32_t off();

      // interfaces for eye color control
      void updateEyeColor(uint32_t color);
      void removeEyeColor();
      void deactivateEyeLeds();
      void activateEyeLeds();

      // interfaces for status LED color control
      void updateStatusLed(uint32_t color);
      void removeStatusColor();
      void deactivateStatusLed();
      void activateStatusLed();

  private:
      Adafruit_NeoPixel leds;
      // leds
      uint16_t leftEye;
      uint16_t rightEye;
      uint16_t statusLed;
      
      void showAllColors();
};


#endif /* LEDHEAD_H_ */
