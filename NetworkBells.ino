#include <SoftwareSerial.h>
#include "pitches.h"
#include "Player.h"
#include "Songs.h"

const int buzzer = 12;//the pin of the active buzzer
const int interruptPin = 2;

SoftwareSerial mySerial(6, 7); // RX, TX

#define BAUD_RATE 115200

#define COMMANDS_LEN 4
const char commands[COMMANDS_LEN][50] = {
  "AT+CWMODE=1",
  "AT+CWJAP=\"NitroSauce\",\"123456789\"",
  "AT+CIPMUX=1",
  "AT+CIPSERVER=1,80"
//  ,"AT+CIFSR"
};




void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

 Serial.println("Running!!!"); 

//  mySerial.begin(115200);
//  mySerial.println("AT+CIOBAUD=9600");
//  waitForOK();

  mySerial.begin(9600);

  for(int i=0;i<COMMANDS_LEN;i++) {
    mySerial.println(commands[i]);
    delay(200);
    waitForOK();
  }
  
  while(!expect("0,CONNECT")) {}
  Serial.println("Got connection!!!"); 

  Player player(buzzer);

  player.playTune(songs::jingleBellsLength, songs::jingleBells);

}


bool expect(char *expected) {
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

void waitForOK() {
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

void loop() { // run over and over

}

