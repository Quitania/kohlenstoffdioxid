#include <WiFiUDP.h>

// the IP address of your InfluxDB host
byte host[] = {192, 168, 178, 201};

// the port that the InfluxDB UDP plugin is listening on
int port = 8089;

WiFiUDP udp;

void sendToDatabase(String data) {
  // send the packet
  Serial.println("Sending UDP packet...");
  udp.beginPacket(host, port);
  udp.print(data);
  udp.endPacket();
}

String createLine(String operationMode, String fieldKey, String fieldValue) {
  return String("indoor-air-quality,sensor=SGP30,operationMode=" + operationMode + " " + fieldKey + "=" + fieldValue + "i");
}
