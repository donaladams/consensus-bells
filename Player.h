#ifndef Player_h
#define Player_h

#include "Arduino.h"

class Player
{
  public:
    Player(int pin);
    void play(int pitch, int duration);
    void playTune(int tuneLength, int tune[][2]);
  private:
    int _pin;
};


#endif
