/*
 * LED Head - LedHead.h
 * (c) by ringbuchblock
 */
#ifndef LEDHEAD_H_
#define LEDHEAD_H_

#include <stdint.h>

class LedHead {

  public:  
      LedHead();
      ~LedHead();

      // pre-defined colors
      uint32_t red();
      uint32_t orange();
      uint32_t yellow();
      uint32_t green();
      uint32_t blue();
      uint32_t violet();
      uint32_t off();

      uint32_t color(uint8_t r, uint8_t g, uint8_t b);

      void updateEyeColor(uint32_t color);
      void removeEyeColor();
      void deactivateEyeLeds();
      void activateEyeLeds();

      void updateStatusLed(uint32_t color);
      void removeStatusColor();
      void deactivateStatusLed();
      void activateStatusLed();

  private:
      void showAllColors();
};


#endif /* LEDHEAD_H_ */
