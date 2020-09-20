#include <Arduino.h>
#include "OgamiHardware.h"
/****************
   LEDS CONTROL - TO DO: add pwm control
****************/


OgamiHardware::OgamiHardware() {
}
void OgamiHardware::ledSetup(int dataPin, int clockPin, int latchPin) {
	setPins(dataPin, clockPin, latchPin);
	setBitCount(8);
	setAll(LOW);
	delay(500);
	set(4,HIGH);
	delay(300);
	set(3,HIGH);
	delay(300);
	set(0,HIGH);
	delay(150);
	set(1,HIGH);
	delay(150);
	set(2,HIGH);
	delay(300);
	setAll(LOW);
	delay(100);
	setAll(HIGH);
	delay(100);
	setAll(LOW);
}
void OgamiHardware::setBitCount(int bitCount) {
	this->bitCount = bitCount;
	this->byteCount = bitCount/8;
	for(int i = 0; i < this->byteCount; i++) {
		this->writeBuffer[i] = 0;
		this->dataModes[i] = 0;
		this->readBuffer[i] = 0;
	}
}

void OgamiHardware::setPins(int dataPin, int clockPin, int latchPin) {
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
	this->dataPin = dataPin;
	this->clockPin = clockPin;
	this->latchPin = latchPin;
}

void OgamiHardware::batchWriteBegin() {
	batchWriteMode = true;
}

void OgamiHardware::set(int bitnum, bool value) {
	if(batchWriteMode) {
		writeBitSoft(bitnum, value);
	} else {
		writeBitHard(bitnum, value);
	}
}

void OgamiHardware::setAll(bool value) {
	for (byte i = 0; i <= bitCount; i++) {
		if(batchWriteMode) {
			writeBitSoft(i, value);
		} else {
			writeBitHard(i, value);
		}
	}
}
void OgamiHardware::batchWriteEnd() {
	writeAllBits();
	batchWriteMode = false;
}

void OgamiHardware::setBitMode(int bitnum, bool mode) {
	int bytenum = bitnum / 8;
	int offset = bitnum % 8;
	byte b = this->dataModes[bytenum];
	bitSet(b, offset);
	this->dataModes[bytenum] = b;
}

void OgamiHardware::writeBitSoft(int bitnum, bool value) {
	int bytenum = bitnum / 8;
	int offset = bitnum % 8;
	byte b = this->writeBuffer[bytenum];
	bitWrite(b, offset, value);
	this->writeBuffer[bytenum] = b;
}

void OgamiHardware::writeBitHard(int bitnum, bool value) {
	writeBitSoft(bitnum, value);
	writeAllBits();
}

void OgamiHardware::writeAllBits() {
	digitalWrite(latchPin, LOW);
	digitalWrite(clockPin, LOW);
	for(int i = 0; i < this->byteCount; i++) {
		shiftOut(dataPin, clockPin, MSBFIRST, this->writeBuffer[i]);
	}
	digitalWrite(latchPin, HIGH);
}

void OgamiHardware::selectorLed(){
	// if (clockSelector > intervalSelector) {
	// 	_selectorValue = selectorValue;
	// 	displayBPM(bpm);
	// }
	// if (_selectorValue != selectorValue) {
	// 	clockSelector = 0;
	displayPreset();
	//}
}


void OgamiHardware::displayPreset(){
	switch (selectorValue) {
	case 0:
		set(0, HIGH);
		set(1, LOW);
		set(2, LOW);
		break;
	case 1:
		set(0, LOW);
		set(1, HIGH);
		set(2, LOW);
		break;
	case 2:
		set(0, LOW);
		set(1, LOW);
		set(2, HIGH);
		break;
	case 3:
		set(0, HIGH);
		set(1, HIGH);
		set(2, LOW);
		break;
	case 4:
		set(0, LOW);
		set(1, HIGH);
		set(2, HIGH);
		break;
	case 5:
		set(0, HIGH);
		set(1, LOW);
		set(2, HIGH);
		break;
	}
}

void OgamiHardware::displayBPM(int bpm){
	clockBPM = 0;
	unsigned int quarterNote = 60000 / bpm;
	if (clockBPM <= 3) {
		set(0, HIGH);
		set(1, LOW);
		set(2, LOW);
	}
	if (clockBPM >= (quarterNote * 0.25)) {
		set(0, LOW);
		set(1, HIGH);
		set(2, LOW);
	}
	if (clockBPM >= (quarterNote * 0.5)) {
		set(0, LOW);
		set(1, LOW);
		set(2, HIGH);
	}
	if (clockBPM >= (quarterNote * 0.75)) {
		set(0, LOW);
		set(1, HIGH);
		set(2, LOW);
	}
	if (clockBPM >= (quarterNote)) {
		set(0, HIGH);
		set(1, LOW);
		set(2, LOW);
	}
}

//-------------------------MULTIPLEXERS----------------------------

void OgamiHardware::muxSetup(int analogDataPin, int digitalDataPin, unsigned int dbt) {
	_analogDataPin = analogDataPin;
	_digitalDataPin = digitalDataPin;
	_dbt = dbt;
	pinMode(5, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(analogDataPin, INPUT);
	pinMode(digitalDataPin, INPUT_PULLUP);
	pinMode(arcadePin, INPUT_PULLUP); //arcade button
	pinMode(expressionPin, INPUT_PULLUP); // exp pedal
	analogReadResolution(10);
}

void OgamiHardware::update() {
	if (clockInputs >= intervalInputs) {
		for (byte i = 0; i<maxInputs; i++)
		{
			readAnalog(i);
			readDigital(i);
			readAux();
			makeArray(); //create a new array with all the inputs values
			clockInputs = 0;
			digitalWrite(5, HIGH && (i & B00000001));
			digitalWrite(4, HIGH && (i & B00000010));
			digitalWrite(3, HIGH && (i & B00000100));
			digitalWrite(2, HIGH && (i & B00001000));
		}
	}
}

void OgamiHardware::readAnalog(byte i){
	potentiometersTemp[i] = analogRead(_analogDataPin);
	if (abs(potentiometersTemp[i] - potentiometersBuffer[i]) > _dbt) {
		potentiometersBuffer[i] = potentiometersTemp[i];
		potentiometers[i] = map(potentiometersTemp[i], minValue, maxValue, 127, 0);
		potentiometers[i] = constrain(potentiometers[i], (byte)0, (byte)127);
	}
}

void OgamiHardware::readDigital(byte i){
	buttonStateTemp[i] = digitalRead(_digitalDataPin);
	if (buttonStateTemp[i] != buttonStateLast[i]) {
		clockDebounce = 0;
	}
	if (clockDebounce > debounceDelay) {
		if (buttonStateTemp[i] != buttonState[i]) {
			buttonState[i] = buttonStateTemp[i];
			readMomentary(i);
			toggleState();
		}
	}
	buttonStateLast[i] = buttonStateTemp[i];
}

void OgamiHardware::readAux(){
	//read arcade
	arcadeStateTemp = digitalRead(arcadePin);
	if (arcadeStateTemp != arcadeStateLast) {
		clockDebounce = 0;
	}
	if (clockDebounce > debounceDelay) {
		if (arcadeStateTemp != arcadeState) {
			arcadeState = map(arcadeStateTemp, 0, 1, 0, 127);
		}
	}
	//read expression pedal
	expressionValueTemp = analogRead(expressionPin);
	if (abs(expressionValueTemp - expressionValueBuffer) > _dbt) {
		expressionValueBuffer = expressionValueTemp;
		expressionValue = map(expressionValueTemp, minValue, maxValue, 0, 127);
		expressionValue = constrain(expressionValue, (byte)0, (byte)127);
	}
}

void OgamiHardware::readMomentary(byte i){
	byte momentary = buttonState[i];
	switch (i) {
	case 2: //left momentary
		if (momentary == LOW ) {
			pressTimer = 0; //timer starts
			momentaryChanged = true;
			if (arcadeState == LOW) {
				isArcadePressed = true;
			}
		}
		if (momentary == HIGH && momentaryChanged == true && (pressTimer > longPress)) {
			selectorValue--;
			momentaryChanged = false;
		}
		if (momentary == HIGH && isArcadePressed == true && (pressTimer < longPress)) {
			//ADD EEPROM SAVE ROUTINE
			momentaryChanged = false;
			isArcadePressed = false;
		}
		break;
	case 1:
		if (momentary == LOW ) {
			pressTimer = 0; //timer starts
			momentaryChanged = true;
			if (arcadeState == LOW) {
				isArcadePressed = true;
			}
		}
		if (momentary == HIGH && momentaryChanged == true && (pressTimer > longPress)) {
			selectorValue++;
			momentaryChanged = false;
		}
		break;
	}
	selectorValue = constrain(selectorValue, (byte)0, (byte)4);
}

void OgamiHardware::toggleState(){
//TOGGLE 1
	if (buttonState[4] == LOW) {toggles[0] = 0;}
	else if (buttonState[3] == LOW) {toggles[0] = 127;}
	else {toggles[0] = 64;}
//TOGGLE 2
	if (buttonState[6] == LOW) {toggles[1] = 0;}
	else if (buttonState[5] == LOW) {toggles[1] = 127;}
	else {toggles[1] = 64;}
//TOGGLE 3
	if (buttonState[0] == LOW) {toggles[2] = 0;}
	else if (buttonState[7] == LOW) {toggles[2] = 127;}
	else {toggles[2] = 64;}
}

void OgamiHardware::makeArray() {
	inputValues[0] = potentiometers[1];
	inputValues[1] = potentiometers[2];
	inputValues[2] = potentiometers[3];
	inputValues[3] = potentiometers[4];
	inputValues[4] = potentiometers[5];
	inputValues[5] = potentiometers[6];
	inputValues[6] = potentiometers[0];
	inputValues[7] = toggles[0];
	inputValues[8] = toggles[1];
	inputValues[9] = toggles[2];
	inputValues[10] = arcadeState;
	inputValues[11] = expressionValue;
	inputValues[12] = selectorValue;
}

byte OgamiHardware::get(byte index){
	return inputValues[index];
}

void OgamiHardware::debug() {
	if (clockDebug > 300) {
		Serial.println("Inputs:");
		for (byte j = 0; j < 13; j++) {
			Serial.print(inputValues[j]);
			Serial.print(" ");
		}
		Serial.println();
		clockDebug = 0;
	}
}
