#ifndef midi_h
#define midi_h

#include <Arduino.h>
#include "functions.h"
#include <EEPROM.h>
const int CCcontrols = 127;

typedef void (*paramFunction) (byte value);
paramFunction setParam[CCcontrols]{
  0, 0, 0, 0,
  outGain,
  lowpassFreq,
  lowpassRes,
  highpassFreq,
  highpassRes,
  feedbackGain,
  lowpassFFreq,
  lowpassFRes,
  highpassFFreq,
  highpassFRes,
  wsDrive,
  bcRate,
  bcBits,
  driveWet,
  glitchSize,
  glitchHead1,
  glitchHead2,
  glitchHead3,
  glitchHead4,
  glitchWet
};

byte CCvalues[CCcontrols] = {0};
byte CCvaluesEEPROM[CCcontrols] = {
  0, 0, 0, 0,
  75, //outGain
  102, //lowpassFreq
  2, //lowpassRes
  2, //highpassFreq
  2, //highpassRes
  0, //feedbackGain
  88, //lowpassFFreq
  10, //lowpassFRes
  100, //highpassFFreq
  6, //highpassFRes
  0, //wsDrive
  0, //bcRate
  127, //bcBits
  0, //driveWet
  1, //glitchSize
  0, //glitchJitter
  127, //glitchHead1
  78, //glitchHead2
  65, //glitchHead3
  0, //glitchHead4
  127, //glitchWet
};

bool eepromUpdated = true;

void updateEEPROM(){
  if (eepromUpdated == false) {
    for (int i=0; i <= CCcontrols; i++){
      EEPROM.update(i, CCvalues[i]);
    }
  }
}

void loadEEPROM(){
  for (int i=0; i <= CCcontrols; i++){
    CCvaluesEEPROM[i] = EEEPROM.read(i);
  }
}

void midiCCread(byte channel, byte control, byte value) {
for (int i=0; i<=CCcontrols; i++){
  if (control ==i) {
    CCvalues[i] = value;
    setParam[i](value);
  }
}
// WOULD THIS WORK IN ALTERNATIVE?
	// switch (control) {
  //   for (i=0; i<=CCcontrols; i++){
  //     case i: setparam[i]();
  //   }
  // }
}

void midiSetup() {
  usbMIDI.setHandleControlChange(midiCCread);
  // usbMIDI.setHandleClock(clockMidi); //MIDI SYNC
  // usbMIDI.setHandleStart(clockStart);
  // usbMIDI.setHandleContinue(clockContinue);
  // usbMIDI.setHandleStop(clockStop);
}
#endif
