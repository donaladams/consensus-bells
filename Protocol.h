#ifndef Protocol_h
#define Protocol_h
#include "Arduino.h"
#include "UDP.h"



typedef struct {
  byte connectionId;
  char *msg;
} ReceivedMessage;


class Protocol 
{
  public:
    Protocol(UDP * connection, int connectionId);
    void send(String message);
    ReceivedMessage receive();
  private:
    UDP * _connection;
    int _connectionId;
    
  
};

#endif
