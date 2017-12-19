#include <SoftwareSerial.h>
#include "pitches.h"
#include "Player.h"
#include "Songs.h"
#include "UDP.h"

const int buzzer = 12;//the pin of the active buzzer
const int interruptPin = 2;

const int WIFI_RX = 6;
const int WIFI_TX = 7;

#define BAUD_RATE 115200

const char* message = "Milo is awesome.";
char msgToSend[50];
#define UDP_COMMANDS_LEN 1
char *udpCommands[UDP_COMMANDS_LEN];

void setup() {

  //snprintf(msgToSend, 50, "AT+CIPSEND=1,16\r\n%s", message);
  udpCommands[0] = "AT+CIPSTART=1,\"UDP\",\"192.168.1.107\",7000,7000,2";
  //udpCommands[1] = "AT+CIPSEND=1,16";
  //udpCommands[2] = message,
  //udpCommands[3] = "AT+CIPCLOSE=1";

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Running!!!"); 

  UDP connection(WIFI_RX, WIFI_TX);

  for(int i=0;i<UDP_COMMANDS_LEN;i++) {
    connection.send(udpCommands[i]);
    connection.waitForOK();
    delay(1000);
  }

 // while(!expect("0,CONNECT")) {}
  while(!connection.expect("+IPD")) {}
  Serial.println("Got connection!!!");

  Player player(buzzer);
  player.playTune(songs::jingleBellsLength, songs::jingleBells);
}



void loop() { // run over and over

}

