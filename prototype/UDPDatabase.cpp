#include "UDPDatabase.h"

const char *kTagValuePairsSeparator = ",";
const char *kTagSetFieldSetSeparator = " ";
const char *kFieldKeyValuePairsSeparator = "=";
const char *kIntegerPostfix = "i";
const char *kLineSeparator = "\n";

UDPDatabase::UDPDatabase(IPAddress ipAddress, int port, String measurement, String tags, int* operationMode)
{
  _ipAddress = ipAddress;
  _port = port;
  _measurementAndTags = measurement + kTagValuePairsSeparator + tags;
  _operationMode = operationMode;
}

String UDPDatabase::createLine(String field, int value)
{
  return _measurementAndTags + kTagValuePairsSeparator + "operationMode=" + *_operationMode + kTagSetFieldSetSeparator + field + kFieldKeyValuePairsSeparator + String(value) + kIntegerPostfix;
}

String UDPDatabase::concatenate(String line1, String line2)
{
  return line1 + kLineSeparator + line2;
}

void UDPDatabase::sendLine(String line)
{
  _udp.beginPacket(_ipAddress, _port);
  _udp.print(line);
  _udp.endPacket();
}
