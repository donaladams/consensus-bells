#ifndef Songs_h
#define Songs_h

namespace songs {

  int jingleBellsLength = 28;
  
  // notes in the melody:
  int jingleBells[28][2] = {
    { NOTE_E4, 8}, { NOTE_E4, 8 }, { NOTE_E4, 4},
    { NOTE_E4, 8}, { NOTE_E4, 8 }, { NOTE_E4, 4},
  
    {NOTE_E4, 8}, {NOTE_G4, 8}, {NOTE_C4, 8}, {NOTE_D4, 8}, {NOTE_E4, 2},
  
    {NOTE_F4, 8}, {NOTE_F4, 8}, {NOTE_F4, 8}, {NOTE_F4, 16}, {NOTE_F4, 16}, 
    {NOTE_F4, 8}, {NOTE_E4, 8}, {NOTE_E4, 8}, {NOTE_E4, 16}, {NOTE_E4, 16},
    {NOTE_E4, 8}, {NOTE_D4, 8}, {NOTE_D4, 8}, {NOTE_E4, 8}, {NOTE_D4, 4}, {NOTE_G4, 4}
  };
}

#endif
