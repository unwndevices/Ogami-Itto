/*
   Led.h - Library for simplified control of 74HC595 shift registers.
   Developed and maintained by Timo Denk and contributers, since Nov 2014.
   Additional information is available at https://timodenk.com/blog/shift-register-arduino-library/
   Released into the public domain.
 */
#ifndef OgamiHardware_h
#define OgamiHardware_h

#include <Arduino.h>

class OgamiHardware {

public:
OgamiHardware();
//LED SHIFT REGISTER
void ledSetup(int dataPin, int clockPin, int latchPin);
void setBitCount(int bitCount);
void setPins(int dataPin, int clockPin, int latchPin);
void setBitMode(int bitnum, bool mode);
void batchWriteBegin();
void batchWriteEnd();
void set(int bitnum, bool value);
void setAll(bool value);
void selectorLed();

//MULTIPLEXERS
void muxSetup(int analogDataPin, int digitalDataPin, unsigned int dbt);
void update();
int get(byte pin);
void debug();

private:
//LED SHIFT REGISTER
int dataPin;
int clockPin;
int latchPin;
int bitCount;
int byteCount;
byte writeBuffer[16];
byte dataModes[16];
byte readBuffer[16];
bool batchWriteMode;
bool batchReadMode;
unsigned int selectorValue = 0;
void writeAllBits();
void writeBitHard(int bitnum, bool value);
void writeBitSoft(int bitnum, bool value);

//MULTIPLEXERS
int _analogDataPin;
int _digitalDataPin;
int S0 = 5;
int S1 = 4;
int S2 = 3;
int S3 = 2;
const int maxInputs = 8;
byte pin;
int minValue = 0;
int maxValue = 1010;
unsigned int _dbt; // Deadband threshold
unsigned int potentiometers[8] = {0};
unsigned int potentiometersTemp[8] = {0};// For smoothing purposes
unsigned int potentiometersBuffer[8] = {0};// For smoothing purposes
elapsedMicros clockInputs;
const unsigned int intervalInputs = 100;

elapsedMicros clockDebounce;
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 300;
bool buttonState[8] {LOW};
bool buttonStateTemp[8] {LOW};
bool buttonStateLast[8] {LOW};
unsigned int toggles[3] {0};
elapsedMillis clockDebug;

void readAnalog(byte i);
void readDigital(byte i);
void readMomentary(byte i);
void toggleState();

};

#endif
