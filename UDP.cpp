#include "Arduino.h"
#include "UDP.h"
#include <SoftwareSerial.h>

// Compute the number of elements in an array
#define arraySize(a) sizeof(a)/sizeof(a[0])

const char setupCommands[][40] = {
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

// Map MAC addresses to IPs for static IP assignment
const char macIpLookup[][2][15] = {
  { "a0:20:a6:12:39:32", "192.168.1.11" }
};

// All the IPs we'll open UDP connections to
const char ips[][15] = {
  "192.168.1.10",
  "192.168.1.11",
  "192.168.1.12",
  "192.168.1.13",
  "192.168.1.14"
};

// Initialize a UDP-based network with a fixed set of IP addresses
// and a mesh of up to 5 nodes. Nodes communicate on port 7000.
UDP::UDP(int rx, int tx): 
  mySerial(rx, tx)
{
  Serial.println("--UDP setup starting");

  this->setBaudRate(9600);
  this->initializeNetworking();    
  this->setDeviceIp();
  this->openConnections();

  Serial.println("--UDP setup complete");
}

// We map connectionIds to a unique source port for each connection.
// The destination will always be 7000.
int connectionIdToPort(int connectionId) {
   return 7000 + 2 * connectionId;
}

// Open a UDP Connection to 'port', sourced from a port calculated by
// connectionIdToPort.
int UDP::openConnection(const char *ip, int port, int connectionId) {
  char buf[100];
  snprintf(buf, 100, "AT+CIPSTART=%d,\"UDP\",\"%s\",%d,%d,2", connectionId, ip, port, connectionIdToPort(connectionId));
  Serial.println(buf);
  sendATCommand(buf);
  waitForOK();
  return connectionId;
}

// Transmit some data on a numbered connection
void UDP::send(int connectionId, String message) {
  char atCommand[100];
  snprintf(atCommand, 100, "AT+CIPSEND=%d,%d", connectionId, message.length());
  this-> sendATCommand(atCommand);
  waitForOK();
  sendATCommand(message.c_str());
  waitForOK();
}

void UDP::sendATCommand(const char *message) {
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

    if ((index > 1) && (output[index-1] == '\r') && (output[index] == '\n')) {
      break;
    }
  }
  
  output[index+1] = '\0';
}

// This should be called waitForK <- no O!
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

// We ask the device for its MAC address and then look up the
// address in a lookup table that maps MAC => IP address.
const char *UDP::getDeviceIp() {
  this->sendATCommand("AT+CIFSR");
  char buf[100];
  while(true) {
     this->readLine(buf, 100);

     if(strstr(buf, "+CIFSR:STAMAC") != NULL) {
       break;
     }

     Serial.println("getDeviceIp() Skipping: -" + String(buf) + "-");
  }

  this->waitForOK();

  char _[20];
  char mac[20];
  sscanf(buf, "%[^,],\"%[^\"]\"", _, mac);
  delay(100);

  for(byte i = 0; i < arraySize(macIpLookup); i++) {
    if(strstr(macIpLookup[i][0], mac) != NULL) {
      Serial.println("Using IP: " + String(macIpLookup[i][1]));
      return macIpLookup[i][1];
    }
  }

  Serial.println("Unknown mac=" + String(mac));

  return NULL;
}

// Look up and set our IP from the look up table, using our mac address as the key
void UDP::setDeviceIp() {    

  //this->sendATCommand("AT+CIPSTA_CUR=\"192.168.1.12\",\"192.168.1.1\",\"255.255.255.0\"");
  //this->waitForOK();  
  Serial.println("Setting ip.");
  const char *ip = this->getDeviceIp();
  char setIpCmd[100];
  snprintf(setIpCmd, 100, "AT+CIPSTA_CUR=\"%s\",\"192.168.1.1\",\"255.255.255.0\"", ip);
  Serial.print("=");Serial.print(setIpCmd);Serial.println("=");
  this->sendATCommand(setIpCmd);
  this->waitForOK();
}

// Thump on the device until it likes our baud rate.
void UDP::setBaudRate(int baud) {
  mySerial.begin(115200);
  delay(100);
  mySerial.println("AT+CIOBAUD=9600");
  mySerial.println("AT+CIOBAUD=9600");
  mySerial.println("AT+CIOBAUD=9600");
  delay(100);
  mySerial.begin(baud);
  mySerial.print("AT\r\nAT\r\nAT");
  waitForOK();
}

// Send all the AT commands we need to get networking running properly.
void UDP::initializeNetworking() {
  for(byte i=0; i < arraySize(setupCommands); i++) {
    Serial.println("Attempting: "); Serial.println(setupCommands[i]); 
    this->sendATCommand(setupCommands[i]);
    this->waitForOK();
    delay(50);
  }
}

// Open one UDP session (not really a connection) for each device.
void UDP::openConnections() {
  Serial.println("Opening connections");
  for(byte i=0; i < arraySize(ips); i++) {
    this->openConnection(ips[i], 7000, i);
  }
}
