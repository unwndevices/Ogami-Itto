#ifndef midi_h
#define midi_h

#include <Arduino.h>
#include "functions.h"
#include <EEPROM.h>
const int CCcontrols = 30;
bool eepromUpdate;


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

byte CCvalues[CCcontrols] = {
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

/* only used at the first startup to check if the eeprom has been ever updated,
   as the default eeprom byte value is 255(TRUE) */

void eepromSetup(){
	checkEEPROM();
	loadEEPROM();
}

void checkEEPROM(){
	bool check;
	check = EEPROM.read(0);
	if (check == TRUE) {
		for (int i=0; i <= CCcontrols; i++) {
			EEPROM.update(i, CCvalues[i]);
		}
	}
}

void loadEEPROM(){ //startup routine - loads saved values into CCvaluesEEPROM and sets the parameters
	AudioNoInterrupts(); //disables briefly the audio output
	for (int i=0; i <= CCcontrols; i++) {
		CCvalues[i] = EEPROM.read(i);
		setParam[i](CCvalues[i]); //updates audio objects parameters
	}
	AudioInterrupts(); //enables the audio output, all the parameters gets updated at the same time
}

//
void updateEEPROM(){
	if (eepromUpdate == TRUE) {
		for (int i=0; i <= CCcontrols; i++) {
			EEPROM.update(i, CCvalues[i]);
		}
	}
}

void midiCCread(byte channel, byte control, byte value) {
	for (int i=0; i<=CCcontrols; i++) {
		CCvalues[i] = value; //updates the CCvalues array in case one wants to update the EEPROM
		setParam[i](value); //sets the parameters using the functions array
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
