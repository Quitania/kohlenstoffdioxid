#include "UDPDatabase.h"

UDPDatabase::UDPDatabase(IPAddress ipAddress, int port)
{
  _ipAddress = ipAddress;
  _port = port;
}

void UDPDatabase::sendLine(String line)
{
  _udp.beginPacket(_ipAddress, _port);
  _udp.print(line);
  _udp.endPacket();
}
