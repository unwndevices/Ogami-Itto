#include "src/OgamiHardware.h"
#include "src/OgamiPresets.h"
#include "src/midi.h"
#include "src/AudioSetup.h"
#include "src/config.h"

OgamiHardware ogami; //Declare the hardware Object
OgamiPresets presets; //(number of presets, numbers of cc controls)


void setup() {
	midiSetup();
	audioSetup();
	presets.init(3, 36);
	presets.loadEEPROM(0); // preset 1 (index 0) gets loaded from eeprom
	ogami.ledSetup(9, 10, 11); //LED BitShifter setup (data, clock, latch)
	ogami.muxSetup(A8, 1, 4); //Potentiometer setup (Analog, Digital, deadband)
}

void loop() {
	ogami.update();
	ogami.selectorLed();
	usbMIDI.read();
	// ogami.debug();

}
