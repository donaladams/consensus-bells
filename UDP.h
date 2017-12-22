#ifndef UDP_h
#define UDP_h

#include <AltSoftSerial.h>
#include "Arduino.h"

typedef struct {
  byte connectionId;
  const char *ip;
} IpAndConnection;

class UDP
{
  public:
    UDP(int rx, int tx);
    void waitForOK();
    int openConnection(const char *ip, int port, int connectionId);
    void send(int connectionId, String message);
    void readLine(char *output, int len);
    byte connectionId;
  private:
    AltSoftSerial mySerial;
    void sendATCommand(const char *message);
    IpAndConnection getDeviceIp();
    void setDeviceIp(const char *ip);
    void setBaudRate(int baud);
    void openConnections();
    void initializeNetworking();
};


#endif
