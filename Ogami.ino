#include "src/OgamiHardware.h"
#include "src/OgamiPresets.h"
#include "src/AudioSetup.h"
#include "src/config.h"

OgamiHardware ogami; //Declare the hardware Object
OgamiPresets presets; //(number of presets, numbers of cc controls)

const int CCcontrols = 36;


void midiCCread(byte channel, byte control, byte value) {
	for (int i=0; i >= (CCcontrols-1); i++) {
	  presets.writeCCArray(i, value); //updates the CCvalues array in case one wants to update the EEPROM
		presets.setCCParam(i, value); //sets the parameters using the functions array
	}
}

void midiSetup() {
  usbMIDI.setHandleControlChange(midiCCread);
  // usbMIDI.setHandleClock(clockMidi); //MIDI SYNC
  // usbMIDI.setHandleStart(clockStart);
  // usbMIDI.setHandleContinue(clockContinue);
  // usbMIDI.setHandleStop(clockStop);
}

void setup() {
	midiSetup();
	audioSetup();
	presets.init(3, 36);
	presets.loadEEPROM(0); // preset 1 (index 0) gets loaded from eeprom
	ogami.ledSetup(9, 10, 11); //LED BitShifter setup (data, clock, latch)
	ogami.muxSetup(A8, 1, 4); //Mux setup (Analog, Digital, deadband)
}

void loop() {
	ogami.update();
	ogami.selectorLed();
	usbMIDI.read();
	// ogami.debug();

}
