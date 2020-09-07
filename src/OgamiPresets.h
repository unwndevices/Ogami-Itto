#ifndef OgamiPresets_h
#define OgamiPresets_h

#include <Arduino.h>
#include "functions.h"
#include <EEPROM.h>
#include <Audio.h>

class OgamiPresets {

public:
OgamiPresets();
void loadEEPROM(int index);
void init(int presets, int CCcontrols);
void updateEEPROM(int index);

byte CCvalues[3][36]{};
typedef void (*paramFunction) (byte value);
paramFunction setParam[36]{
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
int presets;
int index;

private:
int CCcontrols;
bool check;

byte CCvaluesDef[36] = {
	0, 0, 0, 0,
	75,         //outGain
	102,         //lowpassFreq
	2,         //lowpassRes
	2,         //highpassFreq
	2,         //highpassRes
	0,         //feedbackGain
	88,         //lowpassFFreq
	10,         //lowpassFRes
	100,         //highpassFFreq
	6,         //highpassFRes
	0,         //wsDrive
	0,         //bcRate
	127,         //bcBits
	0,         //driveWet
	1,         //glitchSize
	0,         //glitchJitter
	127,         //glitchHead1
	78,         //glitchHead2
	65,         //glitchHead3
	0,         //glitchHead4
	127,         //glitchWet
};

};
extern OgamiPresets EepromVal;

#endif
