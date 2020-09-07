#ifndef midi_h
#define midi_h

#include <Arduino.h>
#include <EEPROM.h>
#include "OgamiPresets.h"
const int CCcontrols = 36;

void midiCCread(byte channel, byte control, byte value) {
	for (int i=0; i >= (CCcontrols-1); i++) {
		EepromVal.CCvalues[EepromVal.index][i] = value; //updates the CCvalues array in case one wants to update the EEPROM
		EepromVal.setParam[i](value); //sets the parameters using the functions array
	}
}

void midiSetup() {
	usbMIDI.setHandleControlChange(midiCCread);
	// usbMIDI.setHandleClock(clockMidi); //MIDI SYNC
	// usbMIDI.setHandleStart(clockStart);
	// usbMIDI.setHandleContinue(clockContinue);
	// usbMIDI.setHandleStop(clockStop);
}
#endif
