#include "pitches.h"
//#include "Player.h"
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

  // Initiate an open protocol with each of the known devices
  Protocol devices[] = {
    Protocol(&connection, 0),
    Protocol(&connection, 1),
    Protocol(&connection, 2),
    Protocol(&connection, 3),
    Protocol(&connection, 4)
  };

  //protocol.send("Hello");
  
  while(true) {
    ReceivedMessage msg = devices[0].receive(); // We ALWAYS receive on the 0 connection
    
    Serial.print("Received "); Serial.print(msg.msg); Serial.println(" from " + String(msg.connectionId));
    if(msg.msg.equals("jingle")) {
      devices[msg.connectionId].send("bells");
    } else if(msg.msg.equals("bells")) {
      devices[msg.connectionId].send("jingle");
    } else {
      devices[msg.connectionId].send("don't know that one.");
    }
  }
}

void loop() { // run over and over
//  Serial.println("In the loop!");
  //Player player(buzzer);
  //player.playTune(songs::jingleBellsLength, songs::jingleBells);
}

