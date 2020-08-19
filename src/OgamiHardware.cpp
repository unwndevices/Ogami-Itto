/*
   Led.cpp - Library for simplified control of 74HC595 shift registers.
   Developed and maintained by Timo Denk and contributers, since Nov 2014.
   Additional information is available at https://timodenk.com/blog/shift-register-arduino-library/
   Released into the public domain.
 */

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
	analogReadResolution(10);
}

void OgamiHardware::update() {
	if (clockInputs >= intervalInputs) {
		for (byte i = 0; i<maxInputs; i++)
		{
			readAnalog(i);
			readDigital(i);
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
	if(abs(potentiometersTemp[i] - potentiometersBuffer[i]) > _dbt) {
		potentiometersBuffer[i] = potentiometersTemp[i];
		potentiometers[i] = map(potentiometersTemp[i], minValue, maxValue, 127, 0);
		potentiometers[i] = constrain(potentiometers[i], (unsigned int)0, (unsigned int)127);
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

void OgamiHardware::readMomentary(byte i){
	bool momentary = buttonState[i];
	switch (i) {
	case 2:
		if (momentary == LOW) {
			selectorValue--;
		}
		break;
	case 1:
		if (momentary == LOW) {
			selectorValue++;
		}
		break;
	}
	selectorValue = constrain(selectorValue, (unsigned int)0, (unsigned int)5);
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

int OgamiHardware::get(byte pin){
	return potentiometers[pin];
}

void OgamiHardware::debug() {
	if (clockDebug > 300) {
		Serial.println("Potentiometers:");
		for (byte j = 0; j < maxInputs; j++) {
			Serial.print(potentiometers[j]);
			Serial.print(" ");
		}
		Serial.println();

		Serial.println("Toggles:");
		for (byte j = 0; j < 3; j++) {
			Serial.print(toggles[j]);
			Serial.print(" ");
		}
		Serial.println();
		Serial.println("selectorValue:");
		Serial.print(selectorValue);
		Serial.println();
		clockDebug = 0;
	}
}
