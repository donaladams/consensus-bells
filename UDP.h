#ifndef UDP_h
#define UDP_h

#include <SoftwareSerial.h>
#include "Arduino.h"

class UDP
{
  public:
    UDP(int rx, int tx);
    void waitForOK();
    bool expect(char *expected);
    int openConnection(String ip, int port);
    void send(int connectionId, String message);
    void readLine(char *output, int len);
  private:
    SoftwareSerial mySerial;
    void sendATCommand(String message);
};


#endif
