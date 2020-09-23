#ifndef Config_h
#define Config_h

#include <FS.h>
#include <LittleFS.h>
#include "shared.h"

boolean writeToFile(SensorConfiguration &sensorConfiguration) {
  File file = LittleFS.open("co2-monitor.conf", "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return false;
  }

  file.write((byte *)&sensorConfiguration, sizeof(sensorConfiguration));
  file.close();

  return true;
}

boolean readFromFile(SensorConfiguration &sensorConfiguration) {
  File file = LittleFS.open("co2-monitor.conf", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return false;
  }

  file.read((byte *)&sensorConfiguration, sizeof(sensorConfiguration));
  file.close();

  return true;
  
}

#endif
