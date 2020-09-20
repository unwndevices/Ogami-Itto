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
byte get(byte index);
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
byte selectorValue = 0;
byte _selectorValue;
// long/short presses
elapsedMillis holdTime;
const int shortPressTime = 480;


const unsigned int intervalSelector = 1000;
elapsedMillis clockSelector;
elapsedMillis clockBPM;
void writeAllBits();
void writeBitHard(int bitnum, bool value);
void writeBitSoft(int bitnum, bool value);
void displayPreset();
void displayBPM(int bpm);

//MULTIPLEXERS
int _analogDataPin;
int _digitalDataPin;
int S0 = 5;
int S1 = 4;
int S2 = 3;
int S3 = 2;
const int maxInputs = 8;
byte index;
int minValue = 0;
int maxValue = 1010;
unsigned int _dbt; // Deadband threshold
byte potentiometers[8] = {0};
byte potentiometersTemp[8] = {0};// For smoothing purposes
byte potentiometersBuffer[8] = {0};// For smoothing purposes
elapsedMicros clockInputs;
const unsigned int intervalInputs = 100;

elapsedMicros clockDebounce;
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 300;
byte buttonState[8] {HIGH};
byte buttonStateTemp[8] {HIGH};
byte buttonStateLast[8] {HIGH};
byte toggles[3] {0};
elapsedMillis clockDebug;

//long press
elapsedMillis pressTimer;
unsigned long longPress = 500;
bool isArcadePressed = false;
bool momentaryChanged = false;

//aux
const int arcadePin = 14;
const int expressionPin = 15;
byte arcadeState = HIGH;
byte arcadeStateTemp = HIGH;
byte arcadeStateLast = HIGH;
byte expressionValue = 0;
byte expressionValueTemp;
byte expressionValueBuffer;

//get array
byte inputValues[13] {0}; // 7 pots + 3 switches + selector + arcade + exp
void makeArray();

void readAnalog(byte i);
void readDigital(byte i);
void readAux();
void readMomentary(byte i);
void toggleState();

};

extern OgamiHardware State;

#endif
