#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
#include "TinyWireM.h"
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#include "PowerMeter2BRobots.h"

PowerMeter2BRobots::PowerMeter2BRobots(uint8_t addr) {
	_i2caddr = addr;
}

boolean PowerMeter2BRobots::begin() {
	Wire.begin();

	if (getID() != PWMT_VALID_ID) return false;

	return true;
}

uint8_t PowerMeter2BRobots::getID(void) {
	return read8(PWMT_REG_ID);
}

void PowerMeter2BRobots::SAVE() {
	write8(PWMT_REG_SAVE, 0X01);
}

void PowerMeter2BRobots::RESET() {
	write8(PWMT_REG_RESET, 0X01);
}

void PowerMeter2BRobots::setCtRatio(int16_t value) {
	write16(PWMT_REG_ctRatio, value);
}

int16_t PowerMeter2BRobots::getCtRatio(void) {
	return read16(PWMT_REG_ctRatio);
}

void PowerMeter2BRobots::setCtRl(int16_t value) {
	write16(PWMT_REG_ctRl, value);
}

int16_t PowerMeter2BRobots::getCtRl(void) {
	return read16(PWMT_REG_ctRl);
}

float PowerMeter2BRobots::getTrRatio(void) {
	conv temp;
	temp.i = read32(PWMT_REG_trRatio);
	return temp.f;
}

void PowerMeter2BRobots::setTrRatio(float value) {
	conv temp;
	temp.f = value;
	write32(PWMT_REG_trRatio, temp.i);
}

void PowerMeter2BRobots::setSamples(int16_t value) {
	write16(PWMT_REG_samples, value);
}

int16_t PowerMeter2BRobots::getSamples(void) {
	return read16(PWMT_REG_samples);
}

float PowerMeter2BRobots::getIrms(void) {
	conv temp;
	temp.i = read32(PWMT_REG_Irms);
	return temp.f;
}

float PowerMeter2BRobots::getVrms(void) {
	conv temp;
	temp.i = read32(PWMT_REG_Vrms);
	return temp.f;
}

float PowerMeter2BRobots::getPower(void) {
	conv temp;
	temp.i = read32(PWMT_REG_power);
	return temp.f;
}

float PowerMeter2BRobots::getAvPower(void) {
	conv temp;
	temp.i = read32(PWMT_REG_avPower);
	return temp.f;
}

void PowerMeter2BRobots::write32(uint8_t reg, uint32_t value) {
	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.write(value & 0xFF);
	Wire.write(value >> 8);
	Wire.write(value >> 16);
	Wire.write(value >> 24);
	Wire.endTransmission();
}

uint32_t PowerMeter2BRobots::read32(uint8_t reg) {
	uint32_t val;

	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.endTransmission();

	Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)4);

	uint8_t buffer[4];
	for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
		buffer[i] = Wire.read();
	}
	val = (uint32_t)buffer[3] << 24;
	val |= (uint32_t)buffer[2] << 16;
	val |= (uint32_t)buffer[1] << 8;
	val |= (uint32_t)buffer[0] & 0xFF;
	return val;
}

void PowerMeter2BRobots::write16(uint8_t reg, uint16_t value) {
	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.write(value & 0xFF);
	Wire.write(value >> 8);
	Wire.endTransmission();
}

uint16_t PowerMeter2BRobots::read16(uint8_t reg) {
	uint16_t val;

	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.endTransmission();

	Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)2);
	val = Wire.read();
	val |= Wire.read() << 8;
	return val;
}

void PowerMeter2BRobots::write8(uint8_t reg, uint8_t value) {
	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.write(value);
	Wire.endTransmission();
}

uint8_t PowerMeter2BRobots::read8(uint8_t reg) {
	uint8_t val;

	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.endTransmission();

	Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)1);
	val = Wire.read();
	return val;
}