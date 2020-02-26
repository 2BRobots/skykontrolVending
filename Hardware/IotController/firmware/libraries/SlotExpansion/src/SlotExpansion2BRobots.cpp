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

#include "SlotExpansion2BRobots.h"

SlotExpansion2BRobots::SlotExpansion2BRobots(uint8_t addr) {
	_i2caddr = addr;
}

boolean SlotExpansion2BRobots::begin() {
	Wire.begin();

	if (getID() != SLE_VALID_ID) return false;

	return true;
}

uint8_t SlotExpansion2BRobots::getID(void) {
	return read8(SLE_REG_ID);
}

void SlotExpansion2BRobots::SAVE() {
	write8(SLE_REG_SAVE, 0X01);
}

void SlotExpansion2BRobots::RESET() {
	write8(SLE_REG_RESET, 0X01);
}

void SlotExpansion2BRobots::startDispensing() {
	write8(SLE_REG_dispense, 0X01);
}

void SlotExpansion2BRobots::stopDispensing() {
	write8(SLE_REG_dispense, 0X00);
}

uint8_t SlotExpansion2BRobots::isDispensing() {
	return read8(SLE_REG_dispense);
}

uint8_t SlotExpansion2BRobots::getSelected() {
	return read8(SLE_REG_selected);
}

void SlotExpansion2BRobots::setSelected(uint8_t value) {
        if(value > 0x04)
        { 
          value = 0x04;
        }
	write8(SLE_REG_selected, value);
}

uint8_t SlotExpansion2BRobots::getCancel() {
	return read8(SLE_REG_cancel);
}

void SlotExpansion2BRobots::setCancel(uint8_t value) {
	write8(SLE_REG_cancel, value & 0x01);
}

uint32_t SlotExpansion2BRobots::getCount() {
	return read32(SLE_REG_counter);
}

void SlotExpansion2BRobots::setCount(uint32_t value) {
	write32(SLE_REG_counter, value);
}

uint32_t SlotExpansion2BRobots::getTimer() {
	return read32(SLE_REG_time);
}

void SlotExpansion2BRobots::setTimer(uint32_t value) {
	write32(SLE_REG_time, value);
}

uint8_t SlotExpansion2BRobots::getCalibrate() {
	return read8(SLE_REG_calibrate);
}

void SlotExpansion2BRobots::setCalibrate(uint8_t value) {
	write8(SLE_REG_calibrate, value & 0x01);
}

uint8_t SlotExpansion2BRobots::getRelay(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read8(SLE_REG_relay1);
		break;
	case 2:
		return read8(SLE_REG_relay2);
		break;
	case 3:
		return read8(SLE_REG_relay3);
		break;
	case 4:
		return read8(SLE_REG_relay4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::setRelay(uint8_t slot, uint8_t value) {
	switch (slot)
	{
	case 1:
		write8(SLE_REG_relay1, value & 0x01);
		break;
	case 2:
		write8(SLE_REG_relay2, value & 0x01);
		break;
	case 3:
		write8(SLE_REG_relay3, value & 0x01);
		break;
	case 4:
		write8(SLE_REG_relay4, value & 0x01);
		break;
	default:
		break;
	}
}

uint8_t SlotExpansion2BRobots::isSelected(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read8(SLE_REG_selected1);
		break;
	case 2:
		return read8(SLE_REG_selected2);
		break;
	case 3:
		return read8(SLE_REG_selected3);
		break;
	case 4:
		return read8(SLE_REG_selected4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::changeSelected(uint8_t slot, uint8_t value) {
	switch (slot)
	{
	case 1:
		write8(SLE_REG_selected1, value & 0x01);
		break;
	case 2:
		write8(SLE_REG_selected2, value & 0x01);
		break;
	case 3:
		write8(SLE_REG_selected3, value & 0x01);
		break;
	case 4:
		write8(SLE_REG_selected4, value & 0x01);
		break;
	default:
		break;
	}
}

String SlotExpansion2BRobots::getProductID(uint8_t slot) {
	String productID = "";
	switch (slot)
	{
	case 1:
		for (char i = 0; i < 13; i++)
		{
			productID += (char)read8(SLE_REG_product_id1 + i);
		}
		return productID;
		break;
	case 2:
		for (char i = 0; i < 13; i++)
		{
			productID += (char)read8(SLE_REG_product_id2 + i);
		}
		return productID;
		break;
	case 3:
		for (char i = 0; i < 13; i++)
		{
			productID += (char)read8(SLE_REG_product_id3 + i);
		}
		return productID;
		break;
	case 4:
		for (char i = 0; i < 13; i++)
		{
			productID += (char)read8(SLE_REG_product_id4 + i);
		}
		return productID;
		break;
	default:
		return productID;
		break;
	}
}

void SlotExpansion2BRobots::setProductID(uint8_t slot, String value) {
	switch (slot)
	{
	case 1:
		for (char i = 0; i < 13; i++)
		{
			write8(SLE_REG_product_id1 + i, value[i]);
		}
		break;
	case 2:
		for (char i = 0; i < 13; i++)
		{
			write8(SLE_REG_product_id2 + i, value[i]);
		}
		break;
	case 3:
		for (char i = 0; i < 13; i++)
		{
			write8(SLE_REG_product_id3 + i, value[i]);
		}
		break;
	case 4:
		for (char i = 0; i < 13; i++)
		{
			write8(SLE_REG_product_id4 + i, value[i]);
		}
		break;
	default:
		break;
	}
}

String SlotExpansion2BRobots::getName(uint8_t slot) {
	String name = "";
	switch (slot)
	{
	case 1:
		for (char i = 0; i < 21; i++)
		{
			name += (char)read8(SLE_REG_name1 + i);
		}
		return name;
		break;
	case 2:
		for (char i = 0; i < 21; i++)
		{
			name += (char)read8(SLE_REG_name2 + i);
		}
		return name;
		break;
	case 3:
		for (char i = 0; i < 21; i++)
		{
			name += (char)read8(SLE_REG_name3 + i);
		}
		return name;
		break;
	case 4:
		for (char i = 0; i < 21; i++)
		{
			name += (char)read8(SLE_REG_name4 + i);
		}
		return name;
		break;
	default:
		return name;
		break;
	}
}

void SlotExpansion2BRobots::setName(uint8_t slot, String value) {
	switch (slot)
	{
	case 1:
		for (char i = 0; i < 21; i++)
		{
			write8(SLE_REG_name1 + i, value[i]);
		}
		break;
	case 2:
		for (char i = 0; i < 21; i++)
		{
			write8(SLE_REG_name2 + i, value[i]);
		}
		break;
	case 3:
		for (char i = 0; i < 21; i++)
		{
			write8(SLE_REG_name3 + i, value[i]);
		}
		break;
	case 4:
		for (char i = 0; i < 21; i++)
		{
			write8(SLE_REG_name4 + i, value[i]);
		}
		break;
	default:
		break;
	}
}

String SlotExpansion2BRobots::getUnit(uint8_t slot) {
	String unit = "";
	switch (slot)
	{
	case 1:
		for (char i = 0; i < 3; i++)
		{
			unit += (char)read8(SLE_REG_unit1 + i);
		}
		return unit;
		break;
	case 2:
		for (char i = 0; i < 3; i++)
		{
			unit += (char)read8(SLE_REG_unit2 + i);
		}
		return unit;
		break;
	case 3:
		for (char i = 0; i < 3; i++)
		{
			unit += (char)read8(SLE_REG_unit3 + i);
		}
		return unit;
		break;
	case 4:
		for (char i = 0; i < 3; i++)
		{
			unit += (char)read8(SLE_REG_unit4 + i);
		}
		return unit;
		break;
	default:
		return unit;
		break;
	}
}

void SlotExpansion2BRobots::setUnit(uint8_t slot, String value) {
	switch (slot)
	{
	case 1:
		for (char i = 0; i < 3; i++)
		{
			write8(SLE_REG_unit1 + i, value[i]);
		}
		break;
	case 2:
		for (char i = 0; i < 3; i++)
		{
			write8(SLE_REG_unit2 + i, value[i]);
		}
		break;
	case 3:
		for (char i = 0; i < 3; i++)
		{
			write8(SLE_REG_unit3 + i, value[i]);
		}
		break;
	case 4:
		for (char i = 0; i < 3; i++)
		{
			write8(SLE_REG_unit4 + i, value[i]);
		}
		break;
	default:
		break;
	}
}

uint32_t SlotExpansion2BRobots::getTime(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read32(SLE_REG_time1);
		break;
	case 2:
		return read32(SLE_REG_time2);
		break;
	case 3:
		return read32(SLE_REG_time3);
		break;
	case 4:
		return read32(SLE_REG_time4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::setTime(uint8_t slot, uint32_t value) {
	switch (slot)
	{
	case 1:
		write32(SLE_REG_time1, value);
		break;
	case 2:
		write32(SLE_REG_time2, value);
		break;
	case 3:
		write32(SLE_REG_time3, value);
		break;
	case 4:
		write32(SLE_REG_time4, value);
		break;
	default:
		break;
	}
}

uint32_t SlotExpansion2BRobots::getCounter(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read32(SLE_REG_counter1);
		break;
	case 2:
		return read32(SLE_REG_counter2);
		break;
	case 3:
		return read32(SLE_REG_counter3);
		break;
	case 4:
		return read32(SLE_REG_counter4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::setCounter(uint8_t slot, uint32_t value) {
	switch (slot)
	{
	case 1:
		write32(SLE_REG_counter1, value);
		break;
	case 2:
		write32(SLE_REG_counter2, value);
		break;
	case 3:
		write32(SLE_REG_counter3, value);
		break;
	case 4:
		write32(SLE_REG_counter4, value);
		break;
	default:
		break;
	}
}

uint16_t SlotExpansion2BRobots::getCost(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read16(SLE_REG_cost1);
		break;
	case 2:
		return read16(SLE_REG_cost2);
		break;
	case 3:
		return read16(SLE_REG_cost3);
		break;
	case 4:
		return read16(SLE_REG_cost4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::setCost(uint8_t slot, uint16_t value) {
	switch (slot)
	{
	case 1:
		write16(SLE_REG_cost1, value);
		break;
	case 2:
		write16(SLE_REG_cost2, value);
		break;
	case 3:
		write16(SLE_REG_cost3, value);
		break;
	case 4:
		write16(SLE_REG_cost4, value);
		break;
	default:
		break;
	}
}

uint16_t SlotExpansion2BRobots::getQuantity(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read16(SLE_REG_quantity1);
		break;
	case 2:
		return read16(SLE_REG_quantity2);
		break;
	case 3:
		return read16(SLE_REG_quantity3);
		break;
	case 4:
		return read16(SLE_REG_quantity4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::setQuantity(uint8_t slot, uint16_t value) {
	switch (slot)
	{
	case 1:
		write16(SLE_REG_quantity1, value);
		break;
	case 2:
		write16(SLE_REG_quantity2, value);
		break;
	case 3:
		write16(SLE_REG_quantity3, value);
		break;
	case 4:
		write16(SLE_REG_quantity4, value);
		break;
	default:
		break;
	}
}

uint16_t SlotExpansion2BRobots::getStock(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read16(SLE_REG_stock1);
		break;
	case 2:
		return read16(SLE_REG_stock2);
		break;
	case 3:
		return read16(SLE_REG_stock3);
		break;
	case 4:
		return read16(SLE_REG_stock4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::setStock(uint8_t slot, uint16_t value) {
	switch (slot)
	{
	case 1:
		write16(SLE_REG_stock1, value);
		break;
	case 2:
		write16(SLE_REG_stock2, value);
		break;
	case 3:
		write16(SLE_REG_stock3, value);
		break;
	case 4:
		write16(SLE_REG_stock4, value);
		break;
	default:
		break;
	}
}

uint16_t SlotExpansion2BRobots::getCapacity(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read16(SLE_REG_capacity1);
		break;
	case 2:
		return read16(SLE_REG_capacity2);
		break;
	case 3:
		return read16(SLE_REG_capacity3);
		break;
	case 4:
		return read16(SLE_REG_capacity4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::setCapacity(uint8_t slot, uint16_t value) {
	switch (slot)
	{
	case 1:
		write16(SLE_REG_capacity1, value);
		break;
	case 2:
		write16(SLE_REG_capacity2, value);
		break;
	case 3:
		write16(SLE_REG_capacity3, value);
		break;
	case 4:
		write16(SLE_REG_capacity4, value);
		break;
	default:
		break;
	}
}

uint16_t SlotExpansion2BRobots::getEmptyLevel(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read16(SLE_REG_emptyLevel1);
		break;
	case 2:
		return read16(SLE_REG_emptyLevel2);
		break;
	case 3:
		return read16(SLE_REG_emptyLevel3);
		break;
	case 4:
		return read16(SLE_REG_emptyLevel4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::setEmptyLevel(uint8_t slot, uint16_t value) {
	switch (slot)
	{
	case 1:
		write16(SLE_REG_emptyLevel1, value);
		break;
	case 2:
		write16(SLE_REG_emptyLevel2, value);
		break;
	case 3:
		write16(SLE_REG_emptyLevel3, value);
		break;
	case 4:
		write16(SLE_REG_emptyLevel4, value);
		break;
	default:
		break;
	}
}

uint16_t SlotExpansion2BRobots::getFullLevel(uint8_t slot) {
	switch (slot)
	{
	case 1:
		return read16(SLE_REG_fullLevel1);
		break;
	case 2:
		return read16(SLE_REG_fullLevel2);
		break;
	case 3:
		return read16(SLE_REG_fullLevel3);
		break;
	case 4:
		return read16(SLE_REG_fullLevel4);
		break;
	default:
		return 0;
		break;
	}
}

void SlotExpansion2BRobots::setFullLevel(uint8_t slot, uint16_t value) {
	switch (slot)
	{
	case 1:
		write16(SLE_REG_fullLevel1, value);
		break;
	case 2:
		write16(SLE_REG_fullLevel2, value);
		break;
	case 3:
		write16(SLE_REG_fullLevel3, value);
		break;
	case 4:
		write16(SLE_REG_fullLevel4, value);
		break;
	default:
		break;
	}
}

void SlotExpansion2BRobots::write32(uint8_t reg, uint32_t value) {
	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.write(value & 0xFF);
	Wire.write(value >> 8);
	Wire.write(value >> 16);
	Wire.write(value >> 24);
	Wire.endTransmission();
}

uint32_t SlotExpansion2BRobots::read32(uint8_t reg) {
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

void SlotExpansion2BRobots::write16(uint8_t reg, uint16_t value) {
	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.write(value & 0xFF);
	Wire.write(value >> 8);
	Wire.endTransmission();
}

uint16_t SlotExpansion2BRobots::read16(uint8_t reg) {
	uint16_t val;

	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.endTransmission();

	Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)2);
	val = Wire.read();
	val |= Wire.read() << 8;
	return val;
}

void SlotExpansion2BRobots::write8(uint8_t reg, uint8_t value) {
	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.write(value);
	Wire.endTransmission();
}

uint8_t SlotExpansion2BRobots::read8(uint8_t reg) {
	uint8_t val;

	Wire.beginTransmission(_i2caddr);
	Wire.write((uint8_t)reg);
	Wire.endTransmission();

	Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)1);
	val = Wire.read();
	return val;
}