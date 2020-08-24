#ifndef functions_h
#define functions_h

#include <Arduino.h>
#include "AudioSetup.h"
#include "config.h"

void wsMorph(byte value) {
	int i;
	for (i = 0; i < 65; i++) {
		newWaveform[i] = (waveformsTable[waveform_select1][i] * (value)) + (waveformsTable[waveform_select2][i] * (1 - value)) * (ws_drive);
	}
}

void lowpassFreq(byte value){
	value = map((float)value, 0, 127, 30, 15000);
	lowpass.frequency(value);
}

void lowpassRes(byte value){
	value = map((float)value, 0, 127, 0.7, 1);
	lowpass.resonance(value);
}

void highpassFreq(byte value){
	value = map((float)value, 0, 127, 30, 15000);
	highpass.frequency(value);
}

void highpassRes(byte value){
	value = map((float)value, 0, 127, 0.7, 1);
	highpass.resonance(value);
}

void feedbackGain(byte value){
	value = map((float)value, 0, 127, 0, 0.7);
	feedback_mixer.gain(1, value);
}

void lowpassFFreq(byte value){
	value = map((float)value, 0, 127, 30, 15000);
	lowpassF.frequency(value);
}

void lowpassFRes(byte value){
	value = map((float)value, 0, 127, 0.7, 1);
	lowpassF.resonance(value);
}

void highpassFFreq(byte value){
	value = map((float)value, 0, 127, 30, 15000);
	highpassF.frequency(value);
}

void highpassFRes(byte value){
	value = map((float)value, 0, 127, 0.7, 1);
	highpassF.resonance(value);
}

void outGain(byte value){
	value = map((float)value, 0, 127, 0, 1.7);
	L_gain.gain(value);
	R_gain.gain(value);
}

void wsMorphing(byte value){
	value = map((float)value, 0, 127, 0, 1);
	wsMorph(value);
	waveshaper.shape(newWaveform, 65);
}

void wsDrive(byte value){
	value = map((float)value, 0, 127, 1, 5);
	wsMorph(value);
	waveshaper.shape(newWaveform, 65);
}

void bcRate(byte value){
	value = map ((int)value, 0, 127, 1, 64);
	bitcrusher.sampleRate(44100 / value);
}

void bcBits(byte value){
	value = map ((int)value, 0, 127, 1, 16);
	bitcrusher.bits(value);
}

void driveWet(byte value){
	value = map((float)value, 0, 127, 0, 1);
	drive_mixer.gain(0, value);
	drive_mixer.gain(1, 1 - value);
}

void glitchSize(byte value){
	delay_time = map((float)value, 0, 127, 0, 1);
	glitch.set_loop_size( 0, delay_time);
	glitch.set_loop_size( 1, delay_time);
	glitch.set_loop_size( 2, delay_time);
	glitch.set_loop_size( 3, delay_time);
}

void glitchJitter(byte value){
	value = map((float)value, 0, 127, 0, 1);
	glitch.set_jitter( 0, value);
	glitch.set_jitter( 1, value);
	glitch.set_jitter( 2, value);
	glitch.set_jitter( 3, value);
}

void glitchHead1(byte value){
	value = map((float)value, 0, 127, 0, 1);
	L_glitch_mixer.gain(0, value);
	R_glitch_mixer.gain(0, value);
}

void glitchHead2(byte value){
	value = map((float)value, 0, 127, 0, 1);
	L_glitch_mixer.gain(1, value);
	R_glitch_mixer.gain(1, value);
}

void glitchHead3(byte value){
	value = map((float)value, 0, 127, 0, 1);
	L_glitch_mixer.gain(2, value);
	R_glitch_mixer.gain(2, value);
}

void glitchHead4(byte value){
	value = map((float)value, 0, 127, 0, 1);
	L_glitch_mixer.gain(3, value);
	R_glitch_mixer.gain(3, value);
}

void glitchWet(byte value){
	value = map((float)value, 0, 127, 0, 1);
	L_dry_wet_mixer.gain(0, 1 - value);
	L_dry_wet_mixer.gain(1, value);
	R_dry_wet_mixer.gain(0, 1 - value);
	R_dry_wet_mixer.gain(1, value);
}


/*------LFO FUNCTION------*/
// void LFOupdate(float LFOtop, float LFObottom) {
// 	static float LFO = 0;
// 	static unsigned long LFOtime = 0;
// 	static bool LFOdirection = false;
// 	unsigned long currentMicros = micros();
// 	static float LFOrange = 0;
// 	unsigned int LFOspeed = delay_time * 30000;
//
//
// 	if (currentMicros - LFOtime >= LFOspeed) {
// 		LFOtime = currentMicros;
// 		LFOrange = LFOtop - LFObottom;
// 		if (LFOrange < 0) LFOrange = 0;
//
//
// 		L_glitch_mixer.gain(1, L_glitch_head2 * ((LFOrange * LFO) + LFOdepth));
// 		R_glitch_mixer.gain(1, R_glitch_head2 * (1 - ((LFOrange * LFO) + LFOdepth)));
// 		L_glitch_mixer.gain(2, L_glitch_head3 * (1 - ((LFOrange * LFO) + LFOdepth)));
// 		R_glitch_mixer.gain(2, R_glitch_head3 * ((LFOrange * LFO) + LFOdepth));
//
//
// 		// Update LFO
// 		if (LFOdirection == false) { //UP
// 			LFO = (LFO + 0.01);
// 			if (LFO >= 1) {
// 				LFOdirection = true;
// 			}
// 		}
//
// 		if (LFOdirection == true) { //Down
// 			LFO = (LFO - 0.01);
// 			if (LFO <= 0) {
// 				LFOdirection = false;
// 			}
// 		}
// 	}
// }

#endif
