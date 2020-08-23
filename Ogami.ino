#include "src/OgamiHardware.h"
#include "src/midi.h"
#include "src/AudioSetup.h"
#include "src/config.h"

OgamiHardware ogami; //Declare the hardware Object


int bpm = 120;

void setup() {
	midiSetup();
	audioSetup();
	ogami.ledSetup(9, 10, 11); //LED BitShifter setup (data, clock, latch)
	ogami.muxSetup(A8, 1, 4); //Potentiometer setup (data, s0-s4, deadband)
}

void loop() {
	ogami.update();
	ogami.selectorLed(bpm);
	// LFOupdate(LFOrangeTop, LFOdepth);
	usbMIDI.read();
	// ogami.debug();

}
