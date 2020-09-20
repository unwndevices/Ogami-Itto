#ifndef config_h
#define config_h

#define UNWN_DEBUG true
/* CONFIGURATION FILE */
float _value;
int CCcontrols = 36;
int presets = 5;
int currentPreset = 0;
int presetSaveCC = 85;
byte oldPreset = 0;
//eeprom


byte CCvalues[36]{};

//audio
float ws_warp = 0;
float ws_drive = 1;
float ws_arcade = 0;

int waveform_select1 = 1;
int waveform_select2 = 2;

float compThres;
float compRatio;
float compAttk;
float compRel;
float compLimit;

float L_glitch_head2 = 0.3;
float L_glitch_head3 = 0.3;

float R_glitch_head2 = 0.3;
float R_glitch_head3 = 0.3;


//------------------LFO-------------------- TO-DO CC//
unsigned int LFOspeed = 30000;
float LFOdepth = 0.2;
float LFOrangeTop = 1;
#endif
