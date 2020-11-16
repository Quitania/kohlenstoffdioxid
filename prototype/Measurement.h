#ifndef Measurement_h
#define Measurement_h

#include <Arduino.h>

class Measurement
{
  public:
    Measurement(String name, String tags, int* operationMode);
    String createLine(String field, float value);
    String createLine(String field, int value);
    static String concatenate(String line1, String line2);
  private:
    String _measurementAndTags;
    int *_operationMode;
};

#endif
