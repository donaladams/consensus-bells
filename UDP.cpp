#include "Arduino.h"
#include "UDP.h"
#include <SoftwareSerial.h>


#define COMMANDS_LEN 9
const char setupCommands[COMMANDS_LEN][50] = {
  "AT+CWMODE=1",
  "AT+CWJAP=\"NitroSauce\",\"123456789\"",
  "AT+CIPMUX=1",
  "AT+CIPCLOSE=0",
  "AT+CIPCLOSE=1",
  "AT+CIPCLOSE=2",
  "AT+CIPCLOSE=3",
  "AT+CIPCLOSE=4",
  "AT+CIFSR"
};


UDP::UDP(int rx, int tx): 
  mySerial(rx, tx)
{
  mySerial.begin(115200);
  mySerial.println("AT+CIOBAUD=9600");
  waitForOK();

  
  mySerial.begin(9600);
  for(int i=0;i<COMMANDS_LEN;i++) {
    mySerial.println(setupCommands[i]);
    this->waitForOK();
    delay(20);
  }
  
}

void UDP::send(char *message) {
  mySerial.println(message);  
}

void UDP::waitForOK() {
  while(true) {
    while(!mySerial.available()) {}
    while(mySerial.available()) {
      char c = mySerial.read();
      Serial.write(c);
      if (c == 'K') {
        mySerial.read();
        mySerial.read();
        return;
      }
    }
  }
}

bool UDP::expect(char *expected) {
  int len = strlen(expected);
  char buf[20];
  
  for(int i = 0 ; i < len; i++) {
    while(!mySerial.available()) {}
    char c = mySerial.read();
    Serial.print(c);
    buf[i] = c;
    if (strncmp(buf, expected, len) == 0) {
      return true;
    }
  }

  return false;
}

