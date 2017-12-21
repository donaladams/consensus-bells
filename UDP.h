#ifndef UDP_h
#define UDP_h

#include <AltSoftSerial.h>
#include "Arduino.h"

class UDP
{
  public:
    UDP(int rx, int tx);
    void waitForOK();
    int openConnection(const char *ip, int port, int connectionId);
    void send(int connectionId, String message);
    void readLine(char *output, int len);
  private:
    AltSoftSerial mySerial;
    void sendATCommand(const char *message);
    const char *getDeviceIp();
    void setDeviceIp();
    void setBaudRate(int baud);
    void openConnections();
    void initializeNetworking();
};


#endif
