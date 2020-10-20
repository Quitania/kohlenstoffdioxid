#ifndef UDPDatabase_h
#define UDPDatabase_h

#include <WiFiUdp.h>

class UDPDatabase
{
  public:
    UDPDatabase(IPAddress ipAddress, int port);
    void sendLine(String line);
  private:
    IPAddress _ipAddress;
    int _port;
    WiFiUDP _udp;
};

#endif
