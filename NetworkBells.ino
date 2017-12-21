#include "pitches.h"
#include "Player.h"
#include "Songs.h"
#include "UDP.h"
#include "Protocol.h"

const int buzzer = 12;//the pin of the active buzzer
const int interruptPin = 2;

const int WIFI_RX = 8;
const int WIFI_TX = 9;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Running."); 
  UDP connection(WIFI_RX, WIFI_TX);

  Protocol protocol(&connection, 0);

  protocol.send("Hello");
  
  while(true) {
    ReceivedMessage msg = protocol.receive();
    Serial.print("Received "); Serial.print(msg.msg); Serial.println(" from " + String(msg.connectionId));
    if(msg.msg.equals("jingle")) {
      protocol.send("bells");
      break;
    } else {
      protocol.send("don't know that one.");
    }
  }
}

void loop() { // run over and over
//  Serial.println("In the loop!");
  //Player player(buzzer);
  //player.playTune(songs::jingleBellsLength, songs::jingleBells);
}

