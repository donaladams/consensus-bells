#include "Protocol.h"
#include <string.h>

Protocol::Protocol(UDP * connection, int remoteId)  {
  _connection = connection;
  _remoteId = remoteId;
  _self = connection->connectionId;
}

void Protocol::send(String message) {
  String msg = String(_self) + "~" + message + '\r' + '\n';
  _connection->send(_remoteId, msg);
}

ReceivedMessage Protocol::receive() {
  char buf[96];

//  Serial.println("Entering Protocol::receive");
  while(true) {
     _connection->readLine(buf, 96);
     buf[95] = '\0';
     
     if(strchr(buf, '~') != NULL) {
//       Serial.println("Breaking out!");
       break;      
     }

//     Serial.println("Skipping: -" + String(buf) + "-");
  }

  char junk[10];
  byte id;
  char msg[88];
  sscanf(buf, "%[^:]:%d~%s", &junk, &id, &msg);
//  Serial.println("Leaving Protocol::receive");

  return { id, msg };
}
