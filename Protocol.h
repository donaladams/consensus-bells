#ifndef Protocol_h
#define Protocol_h
#include "Arduino.h"
#include "UDP.h"

class Protocol 
{
  public:
    Protocol(UDP * connection, int connectionId);
    void send(String message);
  private:
    UDP * _connection;
    int _connectionId;
    
  
};

#endif
