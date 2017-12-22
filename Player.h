#ifndef Player_h
#define Player_h

#include "Arduino.h"

class Player
{
  public:
    Player(byte pin);
    void play(short pitch, short duration);
    void playTune(byte tuneLength, short tune[][2]);
  private:
    byte _pin;
};


#endif
