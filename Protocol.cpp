#include "Protocol.h"
#include <string.h>

Protocol::Protocol(UDP * connection, int connectionId)  {
  _connection = connection;
  _connectionId = connectionId;
}

void Protocol::send(String message) {
  String msg = String(_connectionId) + "~" + message + '\r' + '\n';
  _connection->send(_connectionId, msg);
}

ReceivedMessage Protocol::receive() {
  char buf[128];

  Serial.println("Entering Protocol::receive");
  while(true) {
     _connection->readLine(buf, 128);
     buf[127] = '\0';
     
     if(strchr(buf, '~') != NULL) {
       Serial.println("Breaking out!");
       break;      
     }

     Serial.println("Skipping: -" + String(buf) + "-");
  }

  char junk[10];
  byte id;
  char msg[253];
  sscanf(buf, "%[^:]:%d~%s", &junk, &id, &msg);
  Serial.println("Leaving Protocol::receive");

  return { id, String(msg) };
}
