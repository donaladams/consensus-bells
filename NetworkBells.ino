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

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Running."); 

  UDP connection(WIFI_RX, WIFI_TX);
  int listenId = connection.openConnection("192.168.1.2", 7000);

  
  while(true) {
    char buf[50];
    connection.readLine(buf, 50);
    Serial.println(buf);
  }

  Player player(buzzer);
  player.playTune(songs::jingleBellsLength, songs::jingleBells);
}



void loop() { // run over and over

}

