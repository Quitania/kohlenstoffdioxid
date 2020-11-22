#include "NeoPixelRing.h"

NeoPixelRing::NeoPixelRing() {
  _strip = new Adafruit_NeoPixel(24, 15, NEO_GRB + NEO_KHZ800);
}

void NeoPixelRing::begin() {
  _strip->begin();
  _strip->setBrightness(1);
  _strip->show();
}

void NeoPixelRing::clear() {
  _strip->clear();
}

void NeoPixelRing::colorPixelsFromValue(int value) {
  int step = 44;
  int pixelIndex = 0;
  int i = 400;
  uint32_t color = _strip->Color(0, 255, 0);
  while (pixelIndex < 24 && i <= value) {
    if (pixelIndex == 5) {
      step++;
    } else if (pixelIndex == 9) {
      step = 66;
      color = _strip->Color(255, 255, 0);
    } else if (pixelIndex == 12) {
      step++;
    } else if (pixelIndex == 18) {
      step = 100;
      color = _strip->Color(255, 0, 0);
    }
    _strip->setPixelColor(_getPixelToColor(pixelIndex), color);
    //Serial.println("pixel=" + String(_getPixelToColor(pixelIndex)) + ",i=" + String(i) + ",step=" + String(step) + ",color=" + String(color));
    i += step;
    pixelIndex++;
  }
  _strip->show();
}

uint16_t NeoPixelRing::_getPixelToColor(uint16_t pixel) {
  if (pixel < 12) {
    return pixel + 12;
  } else {
    return pixel - 12;
  }
}
