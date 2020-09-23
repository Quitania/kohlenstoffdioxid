#include <SPI.h>
#include <ESP8266WiFi.h>
#include <sensirion_common.h>
#include <sgp30.h>
#include "wifi_secrets.h"
#include "shared.h"
#include "UDPDatabase.h"
#include "config.h"

#define NORMAL 0
#define FINDING_BASELINE 1

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;     // the Wifi radio's status

int operationMode;
unsigned long previousMillis = 0;
const long readBaselineInterval = 1 * 60 * 60 * 1000;
const long findingBaselineDuration = 12 * 60 * 60 * 1000;
const long readSensorInterval = 10 * 1000;

byte ipInfluxDB[] = {192, 168, 178, 201};
int portInfluxDB = 8089;

UDPDatabase udpDatabase(ipInfluxDB, portInfluxDB, "indoor-air-quality", "sensor=SGP30", &operationMode);

void setup() {
  Serial.begin(115200);

  Serial.println("Attempting to connect to WiFi...");
  WiFi.mode(WIFI_STA);
  status = WiFi.begin(ssid, pass);

  if (status == WL_CONNECTED) { 
    // if the connection succeeded, print network info
    Serial.println("Connected to network");

    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
  }

  // wait for sensor to initialize
  while (sgp_probe() != STATUS_OK) {
    Serial.println("Initialization of sensor failed");
    while (1);
  }

  // check if valid baseline already exists
  SensorConfiguration lastConfiguration;
  boolean success = readFromFile(lastConfiguration);

  if (success && isConfigurationValid(lastConfiguration)) {
    setBaseline(lastConfiguration.iaqBaseline);
    switchOperationMode(NORMAL);
  } else {
    switchOperationMode(FINDING_BASELINE);
  }
}

bool isConfigurationValid(SensorConfiguration sensorConfiguration) {
  return sensorConfiguration.iaqBaseline > 0;
}

void switchOperationMode(int newMode) {
  Serial.println("Switching operation mode to " + String(newMode));
  operationMode = newMode;
}

void readAndStoreBaseline() {
  u32 iaqBaseline = 0;
  String line;

  // read baseline from sensor
  if (sgp_get_iaq_baseline(&iaqBaseline) != STATUS_OK) {
    Serial.println("get baseline failed!");
  } else {
    Serial.println("get baseline");
    Serial.println(iaqBaseline, HEX);

    // write baseline to config
    SensorConfiguration sensorConfiguration = { iaqBaseline };
    writeToFile(sensorConfiguration);

    if (status == WL_CONNECTED) {
      // send baseline to database
      line = udpDatabase.createLine("baseline", iaqBaseline);
      Serial.println(line);

      udpDatabase.sendLine(line);
    }
  }
}

void setBaseline(u32 iaqBaseline) {
  // set baseline in sensor
  sgp_set_iaq_baseline(iaqBaseline);
  Serial.println("set baseline");
  Serial.println(iaqBaseline, HEX);
}

void loop() {
  String line;
  s16 err = 0;
  u16 tvocPpb, co2eqPpm;

  unsigned long currentMillis = millis();
  switch(operationMode) {
    case NORMAL:
      if (currentMillis - previousMillis >= readBaselineInterval) {
        previousMillis = currentMillis;
        readAndStoreBaseline();
      }
      break;
    case FINDING_BASELINE:
      if (currentMillis - previousMillis >= findingBaselineDuration) {
        previousMillis = currentMillis;
        readAndStoreBaseline();
        switchOperationMode(NORMAL);
      }
      break;
  }

  err = sgp_measure_iaq_blocking_read(&tvocPpb, &co2eqPpm);
  if (err == STATUS_OK && status == WL_CONNECTED) {
      line = udpDatabase.concatenate(udpDatabase.createLine("co2eq", co2eqPpm), udpDatabase.createLine("tvoc", tvocPpb));
      Serial.println(line);

      udpDatabase.sendLine(line);
  }

  delay(readSensorInterval);
}
