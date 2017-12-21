#include "Arduino.h"
#include "UDP.h"
#include <SoftwareSerial.h>


#define COMMANDS_LEN 9
const char setupCommands[COMMANDS_LEN][40] = {
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



//snprintf(msgToSend, 50, "AT+CIPSEND=1,16\r\n%s", message);
//udpCommands[0] = "AT+CIPSTART=1,\"UDP\",\"192.168.1.107\",7000,7000,2";
//udpCommands[1] = "AT+CIPSEND=1,16";
//udpCommands[2] = message,
//udpCommands[3] = "AT+CIPCLOSE=1";

char *ips[5] = {
  "192.168.1.10",
  "192.168.1.11",
  "192.168.1.12",
  "192.168.1.13",
  "192.168.1.14"
};

const String START_CONNECTION_FORMAT = "AT+CIPSTART=%d,\"UDP\",\"%s\",%d,%d,2";

UDP::UDP(int rx, int tx): 
  mySerial(rx, tx)
{
  Serial.println("--UDP setup starting");
  mySerial.begin(115200);
  delay(100);
  mySerial.println("AT+CIOBAUD=9600");
  mySerial.println("AT+CIOBAUD=9600");
  mySerial.println("AT+CIOBAUD=9600");
  delay(100);
  mySerial.begin(9600);
  mySerial.print("AT\r\nAT\r\nAT");
  waitForOK();

  for(int i=0;i<COMMANDS_LEN;i++) {
    Serial.println("Attempting: "); Serial.println(setupCommands[i]); 
    this->sendATCommand(setupCommands[i]);
    this->waitForOK();
    delay(50);
  }

  Serial.println("Setting ip.");
  
  
  this->sendATCommand("AT+CIPSTA_CUR=\"192.168.1.12\",\"192.168.1.1\",\"255.255.255.0\"");
  this->waitForOK();

  Serial.println("Opening connections");
  for(int i=0; i<5; i++) {
    this->openConnection(ips[i], 7000, i);
  }

  Serial.println("--UDP setup complete");
}

int connectionIdToPort(int connectionId) {
   return 7000 + 2 * connectionId;
}

int UDP::openConnection(char *ip, int port, int connectionId) {
  char buf[100];
  snprintf(buf, 100, "AT+CIPSTART=%d,\"UDP\",\"%s\",%d,%d,2", connectionId, ip, port, connectionIdToPort(connectionId));
  Serial.println(buf);
  sendATCommand(buf);
  waitForOK();
  return connectionId;
}

void UDP::send(int connectionId, String message) {
  String atCommand = "AT+CIPSEND=" + String(connectionId) + "," + String(message.length());
  this-> sendATCommand(atCommand.c_str());
  waitForOK();
  sendATCommand(message.c_str());
  waitForOK();
}

void UDP::sendATCommand(char *message) {
  mySerial.println(message);  
}

// Read from serial until the end of a line, terminated NL-CR
// Max line length 256.
void UDP::readLine(char *output, int len) {
  byte index;

  for (index = 0; index < len-1; index++) {
    while (!mySerial.available()) {}
    char c = mySerial.read();

    // Store the char in the output
    output[index] = c;

    if ((output[index-1] == '\r') && (output[index] == '\n')) {
      break;
    }
  }
  
  output[index+1] = '\0';
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

