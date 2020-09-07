#include "src/OgamiHardware.h"
#include "src/OgamiPresets.h"
#include "src/AudioSetup.h"
#include "src/config.h"

OgamiHardware ogami; //Declare the hardware Object
OgamiPresets presets; //(number of presets, numbers of cc controls)

int CCcontrols = 36;

void setup() {
	// usbMIDI.setHandleControlChange(midiCCread);
	// usbMIDI.setHandleClock(clockMidi); //MIDI SYNC
	// usbMIDI.setHandleStart(clockStart);
	// usbMIDI.setHandleContinue(clockContinue);
	// usbMIDI.setHandleStop(clockStop);
	audioSetup();
	presets.init(3, CCcontrols);
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

// void midiCCread(byte channel, byte control, byte value) {
// 	for (int i=0; i >= (CCcontrols-1); i++) {
// 		EepromVal.CCvalues[EepromVal.index][i] = value;         //updates the CCvalues array in case one wants to update the EEPROM
// 		EepromVal.setParam[i](value);         //sets the parameters using the functions array
// 	}
// }
