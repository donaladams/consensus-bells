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
    void send(char *message);
  private:
    int _rx;
    int _tx;
    SoftwareSerial mySerial;

};


#endif
