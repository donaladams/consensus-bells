
#include "Protocol.h"


Protocol::Protocol(UDP * connection, int connectionId)  {
  _connection = connection;
  _connectionId = connectionId;
}

void Protocol::send(String message) {
  String msg = String(_connectionId) + "~" + message + '\r' + '\n';
  _connection->send(_connectionId, msg);
}

ReceivedMessage Protocol::receive() {
  char buf[256];
  while(true) {
     _connection->readLine(buf, 256);
     if(String(buf).indexOf('~') >= 0) {
        break;      
      }

      Serial.println("Skipping: -" + String(buf) + "-");
  }
  String protocolMessage = String(buf);

  char junk[10];
  byte id;
  char msg[253];
  sscanf(buf, "%[^:]:%d~%s", &junk, &id, &msg);

  return { id, String(msg) };
}

