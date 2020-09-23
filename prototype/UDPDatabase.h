#ifndef UDPDatabase_h
#define UDPDatabase_h

#include <WiFiUdp.h>

class UDPDatabase
{
  public:
    UDPDatabase(IPAddress ipAddress, int port, String measurement, String tags, int *operationMode);
    String createLine(String field, int value);
    String concatenate(String line1, String line2);
    void sendLine(String line);
  private:
    IPAddress _ipAddress;
    int _port;
    String _measurementAndTags;
    int *_operationMode;
    WiFiUDP _udp;
};

#endif
