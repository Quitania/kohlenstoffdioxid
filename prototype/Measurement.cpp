#include "Measurement.h"

const char *kTagValuePairsSeparator = ",";
const char *kTagSetFieldSetSeparator = " ";
const char *kFieldKeyValuePairsSeparator = "=";
const char *kIntegerPostfix = "i";
const char *kLineSeparator = "\n";

Measurement::Measurement(String name, String tags, int* operationMode)
{
  _measurementAndTags = name;
  _measurementAndTags += kTagValuePairsSeparator;
  _measurementAndTags += tags;
  _operationMode = operationMode;
}

String Measurement::createLine(String field, float value)
{
  return _measurementAndTags + kTagValuePairsSeparator + "operationMode=" + *_operationMode + kTagSetFieldSetSeparator + field + kFieldKeyValuePairsSeparator + String(value);
}

String Measurement::createLine(String field, int value)
{
  return _measurementAndTags + kTagValuePairsSeparator + "operationMode=" + *_operationMode + kTagSetFieldSetSeparator + field + kFieldKeyValuePairsSeparator + String(value) + kIntegerPostfix;
}

String Measurement::concatenate(String line1, String line2)
{
  return line1 + kLineSeparator + line2;
}
