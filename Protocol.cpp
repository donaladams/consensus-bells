
#include "Protocol.h"


Protocol::Protocol(UDP * connection, int connectionId)  {
  _connection = connection;
  _connectionId = connectionId;
}

void Protocol::send(String message) {
  String msg = String(_connectionId) + "~" + message + '\r'+ '\n';
  _connection->send(_connectionId, msg);
}

