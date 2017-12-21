#include "Arduino.h"
#include "UDP.h"
#include <AltSoftSerial.h>


#define COMMANDS_LEN 9
const String setupCommands[COMMANDS_LEN] = {
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


String ips[5] = {
  "192.168.1.10",
  "192.168.1.11",
  "192.168.1.12",
  "192.168.1.13",
  "192.168.1.14"
};

#define LOOKUP_LEN 1
const char macIpLookup[LOOKUP_LEN][2][25] = {
  { "a0:20:a6:12:39:32", "192.168.1.11" }
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
  char *ip = this->getDeviceIp();
  char setIpCmd[100];
  snprintf(setIpCmd, 100, "AT+CIPSTA_CUR=\"%s\",\"192.168.1.1\",\"255.255.255.0\"", ip);
  Serial.print("=");Serial.print(setIpCmd);Serial.println("="); 
  this->sendATCommand(String(setIpCmd));
  this->waitForOK();

  Serial.println("Opening connections");
  for(int i=0; i<5; i++) {
    this->openConnection(ips[i], 7000, i);
  }

  Serial.println("--UDP setup complete");
}



char *UDP::getDeviceIp() {
  this->sendATCommand("AT+CIFSR");
  char buf[256];
  while(true) {
     this->readLine(buf, 256);
     
     if(strstr(buf, "+CIFSR:STAMAC") != NULL) {
       Serial.println("Breaking out!"); 
       break;      
     }

     Serial.println("getDeviceIp() Skipping: -" + String(buf) + "-");
  }

  this->waitForOK();

  Serial.println("Out!"); 
  Serial.println(buf);

  char junk[20];
  char mac[20];
  sscanf(buf, "%[^,],\"%[^\"]\"", junk, mac);

  delay(100);

  for(int i = 0; i < LOOKUP_LEN; i++) {
    if(strstr(macIpLookup[i][0], mac) != NULL) {
      Serial.println("Using IP: " + String(macIpLookup[i][1]));
      return macIpLookup[i][1]; 
    }
  }

  Serial.println("Unknown mac=" + String(mac));

  return NULL;
}





int connectionIdToPort(int connectionId) {
   return 7000 + 2 * connectionId;
}


int UDP::openConnection(String ip, int port, int connectionId) {
  String startConnectionMsg = "AT+CIPSTART=" + String(connectionId) + ",\"UDP\",\"" + ip +"\"," + String(7000) + "," + String(connectionIdToPort(connectionId)) + ",0";
  sendATCommand(startConnectionMsg);
  waitForOK();
  return connectionId;
}

void UDP::send(int connectionId, String message) {
  String atCommand = "AT+CIPSEND=" + String(connectionId) + "," + String(message.length());
  this-> sendATCommand(atCommand);
  waitForOK();
  sendATCommand(message);
  waitForOK();
}

void UDP::sendATCommand(String message) {
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

