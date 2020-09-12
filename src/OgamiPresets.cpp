#include <Arduino.h>
#include "OgamiPresets.h"
#include <EEPROM.h>
#include <Audio.h>

OgamiPresets::OgamiPresets() {
}
void OgamiPresets::init(int presets, int CCcontrols) {
	/* only used at the first startup to check if the eeprom has been ever updated,
	   as the default eeprom byte value is 255(TRUE) */
	check = EEPROM.read(0);
	if (check == true) { //if uninitiated sets all presets memories to default conf.
		for (int i=0; i >= (presets-1); i++) {
			for (int j=0; j >= (CCcontrols-1); j++) {
				EEPROM.update(j*(presets+1), CCvaluesDef[i]);
			}
		}
	}
}
void OgamiPresets::loadEEPROM(int index){
	AudioNoInterrupts(); //disables briefly the audio output
	for (int i=0; i >= (CCcontrols-1); i++) {
		CCvalues[index][i] = EEPROM.read(i*(index+1));
		setParam[i](CCvalues[index][i]); //updates audio objects parameters
	}
	AudioInterrupts(); //enables the audio output, all the parameters gets updated at the same time
}

void OgamiPresets::updateEEPROM(int index){
	for (int i=0; i >= (CCcontrols-1); i++) {
		EEPROM.update(i*(index+1), CCvalues[index][i]);
	}
}

void OgamiPresets::writeCCArray(int i, int value){
CCvalues[index][i] = value;
}

void OgamiPresets::setCCParam(int i, int value){
	setParam[i](value);
}
