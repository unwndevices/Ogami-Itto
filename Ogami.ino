#include "src/OgamiHardware.h"

OgamiHardware ogami; //Declare the hardware Object
void setup() {
	Serial.begin(9600);
	ogami.ledSetup(9, 10, 11); //LED BitShifter setup (data, clock, latch)
	ogami.muxSetup(A8, 1, 4); //Potentiometer setup (data, s0-s4, deadband)
}

void loop() {
	ogami.update();
	ogami.selectorLed();
	ogami.debug();

}
