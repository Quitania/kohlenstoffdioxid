#ifndef NeoPixelRing_h
#define NeoPixelRing_h

#include <Adafruit_NeoPixel.h>

class NeoPixelRing
{
  public:
    NeoPixelRing();
    void begin();
    void colorPixelsFromValue(int value);
    void clear();
  private:
    Adafruit_NeoPixel *_strip;
    uint16_t _getPixelToColor(uint16_t pixel);
};

#endif
