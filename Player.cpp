
#include "Arduino.h"
#include "Player.h"


Player::Player(int pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
};

void Player::play(int pitch, int duration) {
  tone(_pin, pitch, duration);
   // to distinguish the notes, set a minimum time between them.
  // the note's duration + 30% seems to work well:
  int pauseBetweenNotes = duration * 1.30;
  delay(pauseBetweenNotes);
  // stop the tone playing:
  noTone(8);
};


void Player::playTune(int tuneLength, int tune[][2]) {
 //output an frequency
 for(int thisNote=0;thisNote<tuneLength;thisNote++)
 {
  // to calculate the note duration, take one second divided by the note type.
  //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  int pitch = tune[thisNote][0];
  int duration = 1000 / tune[thisNote][1];
  play(pitch, duration);
 }
};


