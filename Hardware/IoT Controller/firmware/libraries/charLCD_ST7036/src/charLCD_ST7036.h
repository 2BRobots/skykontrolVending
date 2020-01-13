// ---------------------------------------------------------------------------
// Based on ST7306LCD_Library by Francisco Malpartida on 20/08/11.
// https://github.com/pkourany/ST7306LCD_Library
// Copyright 2011 - Under creative commons license 3.0:
//        Attribution-ShareAlike CC BY-SA
//
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//        This library implements the driver to any I2C display with the ST7036
//        LCD controller. 
//        I2C displays based on the ST7032i should also be compatible.
//
//        Other compatible displays:
//           - NHD-C0220BiZ-FSW-FBW-3V3M
//           - NHD-C0220BiZ-FS(RGB)-FBW-3VM
//        Non tested but should be compatible with no or little changes
//           - NHD-C0216CiZ-FSW-FBW-3V3
//           - NHD-C0216CiZ-FN-FBW-3V
//
// @author F. Malpartida - fmalpartida@gmail.com
// Modified and complemented by Danica FR for 2BRobots.
// ---------------------------------------------------------------------------

#ifndef charLCD_ST7036_h
#define charLCD_ST7036_h

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

#define SMART_VALID_ID               0xF3
#define SMART_REG_ID                 0x00
#define SMART_REG_RESET              0x01
#define SMART_REG_DETECTED           0x02
#define SMART_REG_TIMEOUT            0x03
#define SMART_REG_REMAIN             0x07
#define SMART_REG_BEEP               0x0B
#define SMART_REG_BRIGHTNESS         0x0D

class charLCD_ST7036 : public Print 
{
 public:
	charLCD_ST7036(uint8_t num_lines, uint8_t num_col, uint8_t i2cAddr, uint8_t i2cSAddr );
	void command(uint8_t value);
	boolean begin();
        boolean beginSmart();
	void setDelay(int,int);
	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buffer, size_t size);
	void clear();
	void home();
	void display();
	void noDisplay();
	void cursor();
	void noCursor();
	void blink();
	void noBlink();
	void setCursor(uint8_t Line, uint8_t Col );
	uint8_t status();
	void createChar(uint8_t char_num, uint8_t *rows);
	void printstr(const char[]);
        void setContrast(uint8_t contrast);
	void setBrightness(uint16_t brightness);
        void beep(uint16_t beep);
        void reset();
        void setTimeout(uint32_t timeout);
        uint32_t getRemaining();
        uint8_t proxDetected();
        void write32(uint8_t reg, uint32_t val);
        uint32_t read32(uint8_t reg);  
        void write16(uint8_t reg, uint16_t val);
        uint16_t read16(uint8_t reg);
        void write8(uint8_t reg, uint8_t val);
        uint8_t read8(uint8_t reg);
	
 private:
	uint8_t _num_lines;
	uint8_t _num_col;
	uint8_t _i2cAddress;
        uint8_t _i2cSAddress;
	int     _cmdDelay;
	int     _charDelay;
	bool    _initialised;
        bool    _isSmart;
	uint8_t _status;
};

#endif

