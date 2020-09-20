#include "src/OgamiHardware.h"
#include "src/AudioSetup.h"
#include "src/config.h"
#include <EEPROM.h>
OgamiHardware ogami; //Declare the hardware Object


byte CCvaluesDef[5][36] = {
	{
		0, //0
		0, //1
		0, //2
		0, //3
		75, //4 outGain
		102, //5 lowpassFreq
		2, //6 lowpassRes
		2, //7 highpassFreq
		2, //8 highpassRes
		0, //9 feedbackGain
		88, //10 lowpassFFreq
		10, //11lowpassFRes
		100, //12 highpassFFreq
		6, //13 highpassFRes
		0, //14 wsDrive
		0, //15 bcRate
		127, //16 bcBits
		0, //17 driveWet
		1, //18 glitchSize
		0, //19 glitchJitter
		127, //20 glitchHead1
		78, //21 glitchHead2
		65, //22 glitchHead3
		0, //23 glitchHead4
		127, //24 glitchWet
	},
	{
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
	},
	{
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
	},
	{
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
	},
	{
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
	}};




void midiSetup() {
	usbMIDI.setHandleControlChange(midiCCread);
	// usbMIDI.setHandleClock(clockMidi); //MIDI SYNC
	// usbMIDI.setHandleStart(clockStart);
	// usbMIDI.setHandleContinue(clockContinue);
	// usbMIDI.setHandleStop(clockStop);
}

void wsMorph(float _value) {
	int i;
	for (i = 0; i < 65; i++) {
		newWaveform[i] = (waveformsTable[waveform_select1][i] * (_value)) + (waveformsTable[waveform_select2][i] * (1 - _value)) * (ws_drive);
	}
}

void lowpassFreq(byte value){
	_value = map((float)value, 0, 127, 30, 15000);
	lowpass.frequency(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void lowpassRes(byte value){
	_value = map((float)value, 0, 127, 0.7, 1);
	lowpass.resonance(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void highpassFreq(byte value){
	_value = map((float)value, 0, 127, 30, 15000);
	highpass.frequency(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void highpassRes(byte value){
	_value = map((float)value, 0, 127, 0.7, 1);
	highpass.resonance(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void feedbackGain(byte value){
	_value = map((float)value, 0, 127, 0, 0.7);
	feedback_mixer.gain(1, _value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void lowpassFFreq(byte value){
	_value = map((float)value, 0, 127, 30, 15000);
	lowpassF.frequency(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void lowpassFRes(byte value){
	_value = map((float)value, 0, 127, 0.7, 1);
	lowpassF.resonance(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void highpassFFreq(byte value){
	_value = map((float)value, 0, 127, 30, 15000);
	highpassF.frequency(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void highpassFRes(byte value){
	_value = map((float)value, 0, 127, 0.7, 1);
	highpassF.resonance(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void outGain(byte value){
	_value = map((float)value, 0, 127, 0, 1.7);
	L_gain.gain(_value);
	R_gain.gain(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void wsMorphing(byte value){
	_value = map((float)value, 0, 127, 0, 1);
	wsMorph(_value);
	waveshaper.shape(newWaveform, 65);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void wsDrive(byte value){
	_value = map((float)value, 0, 127, 1, 5);
	wsMorph(_value);
	waveshaper.shape(newWaveform, 65);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void bcRate(byte value){
	_value = map ((int)value, 0, 127, 1, 64);
	bitcrusher.sampleRate(44100 / _value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(44100 / _value);
	#endif
}

void bcBits(byte value){
	_value = map ((int)value, 0, 127, 1, 16);
	bitcrusher.bits(_value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void driveWet(byte value){
	_value = map((float)value, 0, 127, 0, 1);
	drive_mixer.gain(0, _value);
	drive_mixer.gain(1, 1 - _value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void glitchSize(byte value){
	_value = map((float)value, 0, 127, 0, 1);
	glitch.set_loop_size( 0, _value);
	glitch.set_loop_size( 1, _value);
	glitch.set_loop_size( 2, _value);
	glitch.set_loop_size( 3, _value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void glitchJitter(byte value){
	_value = map((float)value, 0, 127, 0, 1);
	glitch.set_jitter( 0, _value);
	glitch.set_jitter( 1, _value);
	glitch.set_jitter( 2, _value);
	glitch.set_jitter( 3, _value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void glitchHead1(byte value){
	_value = map((float)value, 0, 127, 0, 1);
	L_glitch_mixer.gain(0, _value);
	R_glitch_mixer.gain(0, _value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void glitchHead2(byte value){
	_value = map((float)value, 0, 127, 0, 1);
	L_glitch_mixer.gain(1, _value);
	R_glitch_mixer.gain(1, _value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void glitchHead3(byte value){
	_value = map((float)value, 0, 127, 0, 1);
	L_glitch_mixer.gain(2, _value);
	R_glitch_mixer.gain(2, _value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void glitchHead4(byte value){
	_value = map((float)value, 0, 127, 0, 1);
	L_glitch_mixer.gain(3, _value);
	R_glitch_mixer.gain(3, _value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void glitchWet(byte value){
	_value = map((float)value, 0, 127, 0, 1);
	L_dry_wet_mixer.gain(0, 1 - _value);
	L_dry_wet_mixer.gain(1, value);
	R_dry_wet_mixer.gain(0, 1 - _value);
	R_dry_wet_mixer.gain(1, value);
	#ifdef UNWN_DEBUG
	Serial.println(__func__);
	Serial.print("raw: ");
	Serial.println(value);
	Serial.print("map: ");
	Serial.println(_value);
	#endif
}

void doNothing(byte value){
	return;
}

typedef void (*paramFunction) (byte value);
paramFunction setParam[36]{
	doNothing, //0
	doNothing, //1
	doNothing, //2
	doNothing, //3
	outGain, //4
	lowpassFreq, //5
	lowpassRes, //6
	highpassFreq, //7
	highpassRes, //8
	feedbackGain, //9
	lowpassFFreq, //10
	lowpassFRes, //11
	highpassFFreq, //12
	highpassFRes, //13
	wsDrive, //14
	bcRate, //15
	bcBits, //16
	driveWet, //17
	glitchSize, //18
	glitchJitter, //19
	glitchHead1, //20
	glitchHead2, //21
	glitchHead3, //22
	glitchHead4, //23
	glitchWet, //24
	doNothing,
	doNothing,
	doNothing,
	doNothing,
	doNothing,
	doNothing,
	doNothing,
	doNothing,
	doNothing,
	doNothing,
	doNothing
};

/*********************EEPROM****************/

void initPreset(int presets, int CCcontrols) {
	/* only used at the first startup to check if the eeprom has been ever updated,
	   as the default eeprom byte value is 255(TRUE) */
	unsigned int check = EEPROM.read(0);

	Serial.print("CHK : ");
	Serial.println(check);

	if (check  == 255) { //if uninitiated sets all presets memories to default conf.
		#ifdef UNWN_DEBUG
		Serial.print("initEEPROM ");
		Serial.print(presets);
		Serial.print(" ");
		Serial.println(CCcontrols);
		#endif
		int c = 0;
		for (int i=0; i <= (presets-1); i++) {
			for (int j=0; j <= (CCcontrols-1); j++) {
				EEPROM.update(c, CCvaluesDef[i][j]);

				/*
				   Serial.print("READ #");
				   Serial.print(c);

				   Serial.print(" AND WRITE PARAM #");
				   Serial.print(c);
				   Serial.print(" with value ");
				   Serial.println(CCvaluesDef[i][j]);
				 */

				c++;

			}




		}
	}
	loadPreset(currentPreset); //loads index 0 preset
}

void loadPreset(int index){



	AudioNoInterrupts(); //disables briefly the audio output

	int startFrom = CCcontrols*(index);
	int endTo = CCcontrols*(index + 1);
	int c = 0;

	#ifdef UNWN_DEBUG
	Serial.print("LOAD PRESET #");
	Serial.println(currentPreset);
	Serial.print("START FROM #");
	Serial.println(startFrom);
	Serial.print("end to  #");
	Serial.println(endTo);
	#endif
	for (int i=startFrom; i < endTo; i++) {

		CCvalues[c] = EEPROM.read(i);


		#ifdef UNWN_DEBUG
		Serial.print("READ #");
		Serial.print(i);
		Serial.print(" AND SET PARAM #");
		Serial.print(c);
		Serial.print(" with value ");
		Serial.println(CCvalues[c]);
		#endif

		setParam[c](CCvalues[c]);

		c++;
	}


	AudioInterrupts(); //enables the audio output, all the parameters gets updated at the same time
}

void updatePreset(int index){
	int startFrom = CCcontrols*(index);
	int endTo = CCcontrols*(index + 1);
	int c = 0;

	for (int i=startFrom; i < endTo; i++) {
		EEPROM.update(i, CCvalues[c]);
		c++;
	}
}

void loopPreset(){ //swap for an interrupt routine
	currentPreset = ogami.get(12);
	if (currentPreset != oldPreset) {
		loadPreset(currentPreset);
	}
	oldPreset = currentPreset;
}
/***************END EEPROM****************/

void midiCCread(byte channel, byte control, byte value) {

	if(control >= 0 && control < CCcontrols) { //control in range 0...36
		CCvalues[control] = value;
		setParam[control](value); //sets the parameters using the functions array
	}

	else{
		if (control == presetSaveCC) { //saves presets to EEPROM when prompted by the configurator
			if (value >=1 && value<=presets) {
				updatePreset(value-1);
			}
		}
	}
}

void interfaceRead(){
}

void setup() {
	midiSetup();
	initPreset(presets, CCcontrols);
	audioSetup();
	ogami.ledSetup(9, 10, 11); //LED BitShifter setup (data, clock, latch)
	ogami.muxSetup(A8, 1, 4); //Mux setup (Analog, Digital, deadband)
}

void loop() {
	ogami.update();
	ogami.selectorLed();
	loopPreset();
	usbMIDI.read();
	// ogami.debug();

}
