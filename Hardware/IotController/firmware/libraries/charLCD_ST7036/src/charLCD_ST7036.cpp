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

#include "charLCD_ST7036.h"

// Class private constants and definition
const int     CMD_DELAY           = 1;  // Command delay in milliseconds
const int     CHAR_DELAY          = 0;  // Delay between characters in milliseconds
const int     PIXEL_ROWS_PER_CHAR = 8;  // Number of pixel rows in the LCD character
const int     MAX_USER_CHARS      = 16; // Maximum number of user defined characters

// LCD Command set
const uint8_t DISP_CMD       = 0x0;  // Command for the display
const uint8_t RAM_WRITE_CMD  = 0x40; // Write to display RAM
const uint8_t CLEAR_DISP_CMD = 0x01; // Clear display command
const uint8_t HOME_CMD       = 0x02; // Set cursor at home (0,0)
const uint8_t DISP_ON_CMD    = 0x0C; // Display on command
const uint8_t DISP_OFF_CMD   = 0x08; // Display off Command
const uint8_t SET_DDRAM_CMD  = 0x80; // Set DDRAM address command
const uint8_t CONTRAST_CMD   = 0x70; // Set contrast LCD command
const uint8_t FUNC_SET_TBL0  = 0x38; // Function set - 8 bit, 2 line display 5x8, inst table 0
const uint8_t FUNC_SET_TBL1  = 0x39; // Function set - 8 bit, 2 line display 5x8, inst table 1

// LCD bitmap definition
const uint8_t CURSOR_ON_BIT  = ( 1 << 1 );// Cursor selection bit in Display on CMD.
const uint8_t BLINK_ON_BIT   = ( 1 << 0 );// Blink selection bit on Display on CMD.

// Driver DDRAM addressing
const uint8_t dram_dispAddr [][3] =
{
	{ 0x00, 0x00, 0x00 },  // One line display address
	{ 0x00, 0x40, 0x00 },  // Two line display address
	{ 0x00, 0x10, 0x20 }   // Three line display address
};


// Constructors:
charLCD_ST7036::charLCD_ST7036(uint8_t num_lines, uint8_t num_col, uint8_t i2cAddr, uint8_t i2cSAddr )
{
	_num_lines    = num_lines;
	_num_col      = num_col;
	_i2cAddress   = i2cAddr & 0x7F;
        _i2cSAddress  = i2cSAddr & 0x7F;
	_cmdDelay     = CMD_DELAY;
	_charDelay    = CHAR_DELAY;
	_initialised  = false;
        _isSmart      = false;
	_status       = 0;
}

// Functions: modifiers (set), selectors (get) and class methods
boolean charLCD_ST7036::begin ()
{
	// Initialize the Wire library.
	Wire.begin();
	
	Wire.beginTransmission ( _i2cAddress );
	Wire.write ( 0x00 );   // Send command to the display
	Wire.write ( FUNC_SET_TBL0 );
	delay (10);
	Wire.write ( FUNC_SET_TBL1 );
	delay (10);
	Wire.write ( 0x14 );  // Set BIAS - 1/5
	Wire.write ( 0x78 );  // Set contrast low byte
	Wire.write ( 0x5E );  // ICON display on, Booster on, Contrast high byte
	Wire.write ( 0x6D );  // Follower circuit (internal), amp ratio (6)
	Wire.write ( 0x0C );  // Display on
	Wire.write ( 0x01 );  // Clear display
	Wire.write ( 0x06 );  // Entry mode set - increment
	_status = Wire.endTransmission ();
	
	if ( _status == 0 )
	{
		_initialised = true;
		return true;
	}
	else
	{
		return false;
	}
}

boolean charLCD_ST7036::beginSmart ()
{
	// Initialize the Wire library.
	Wire.begin();
	
	if (read8(SMART_REG_ID) == SMART_VALID_ID)
	{
		_isSmart = true;
		return true;
	}
	else
	{
		return false;
	}
}

void charLCD_ST7036::setDelay (int cmdDelay,int charDelay)
{
	_cmdDelay = cmdDelay;
	_charDelay = charDelay;
}


void charLCD_ST7036::command(uint8_t value)
{
	// If the LCD has been initialized correctly, write to it
	if ( _initialised )
	{
		Wire.beginTransmission ( _i2cAddress );
		Wire.write ( DISP_CMD );
		Wire.write ( value );
		_status = Wire.endTransmission ();
		delay(_cmdDelay);
	}
}


size_t charLCD_ST7036::write(uint8_t value)
{
	// If the LCD has been initialized correctly write to it
	if ( _initialised )
	{		
		// If it is a new line, set the cursor to the next line (1,0)
		if ( value == '\n' )
		{
			setCursor (1,0);
		}
		else
		{
			Wire.beginTransmission ( _i2cAddress );
			Wire.write ( RAM_WRITE_CMD );
			Wire.write ( value );
			_status = Wire.endTransmission ();
			delay(_charDelay);
		}
		return 1;
	}
	return 0;
}

size_t charLCD_ST7036::write(const uint8_t *buffer, size_t size)
{
	// If the LCD has been initialized correctly, write to it
	if ( _initialised )
	{
		Wire.beginTransmission ( _i2cAddress );
		Wire.write ( RAM_WRITE_CMD );
		Wire.write ( (uint8_t *)buffer, size );
		_status = Wire.endTransmission ();
		delay(_charDelay);
		return size;
	}
	return 0;
}


void charLCD_ST7036::clear()
{
	command (CLEAR_DISP_CMD);
}


void charLCD_ST7036::home()
{
	command ( HOME_CMD );
}


void charLCD_ST7036::display()
{
	command ( DISP_ON_CMD );
}


void charLCD_ST7036::noDisplay()
{
	command ( DISP_OFF_CMD );
}


void charLCD_ST7036::cursor()
{
	command ( DISP_ON_CMD | CURSOR_ON_BIT );
}

void charLCD_ST7036::noCursor()
{
	command ( DISP_ON_CMD & ~(CURSOR_ON_BIT) );
}

void charLCD_ST7036::blink()
{
	command ( DISP_ON_CMD | BLINK_ON_BIT );
}

void charLCD_ST7036::noBlink()
{
	command ( DISP_ON_CMD & ~(BLINK_ON_BIT) );
}


void charLCD_ST7036::setCursor(uint8_t line_num, uint8_t x)
{
	uint8_t base = 0x00;
	// If the LCD has been initialized correctly, write to it
	if ( _initialised )
	{
		// set the baseline address with respect to the number of lines of
		// the display
		base = dram_dispAddr[_num_lines-1][line_num];
		base = SET_DDRAM_CMD + base + x;
		command ( base );
	}
}

uint8_t charLCD_ST7036::status()
{
	return _status;
}



void charLCD_ST7036::createChar (uint8_t char_num, uint8_t *rows)
{
	// If the LCD has been initialized correctly start writing to it
	if ( _initialised )
	{
		// If it is a valid place holder for the character, write it into the displays CGRAM
		if ( char_num < MAX_USER_CHARS )
		{
			// Set up the display to write into CGRAM - configure LCD to use function table 0
			Wire.beginTransmission ( _i2cAddress );
			Wire.write ( DISP_CMD );
			Wire.write ( FUNC_SET_TBL0 ); // Function set: 8 bit, 2 line display 5x8, function table 0
			delay ( _cmdDelay );
			
			// Set CGRAM position to write
			Wire.write ( RAM_WRITE_CMD + (PIXEL_ROWS_PER_CHAR * char_num) );
			_status = Wire.endTransmission ();
			
			// If we have changed the function table and configured the CGRAM position write the new character to the LCD's CGRAM
			if ( _status == 0 )
			{
				write ( rows, PIXEL_ROWS_PER_CHAR ); // write the character to CGRAM			
				// Leave the LCD as it was - function table 1 DDRAM and set the cursor position to (0, 0) to start writing.
				command ( FUNC_SET_TBL1 );
				setCursor ( 0,0 );
			}
		}
	}
}

void charLCD_ST7036::setContrast(uint8_t contrast)
{
	// Only allow 15 levels of contrast
        if (contrast > 15)
        {
           contrast = 15;
        }	
	command(CONTRAST_CMD | contrast);
}

void charLCD_ST7036::setBrightness(uint16_t brightness)
{
	// If the LCD is smart, write to it
	if ( _isSmart )
	{
           if (brightness > 1024)
           {
              brightness = 1023;
           }
           write16(SMART_REG_BRIGHTNESS, brightness);
        }
}

void charLCD_ST7036::beep(uint16_t beep)
{
	// If the LCD is smart, write to it
	if ( _isSmart )
	{
            write16(SMART_REG_BEEP, beep);
        }
}

void charLCD_ST7036::reset()
{
	// If the LCD is smart, write to it
	if ( _isSmart )
	{
            write8(SMART_REG_RESET, 0x01);
        }
}

void charLCD_ST7036::setTimeout(uint32_t timeout)
{
	// If the LCD is smart, write to it
	if ( _isSmart )
	{
            write32(SMART_REG_TIMEOUT, timeout);
        }
}


uint32_t charLCD_ST7036::getRemaining()
{
	// If the LCD is smart, write to it
	if ( _isSmart )
	{
            return read32(SMART_REG_REMAIN);
        }
        return 0x00;
}

uint8_t charLCD_ST7036::proxDetected()
{
	// If the LCD is smart, write to it
	if ( _isSmart )
	{
            return read8(SMART_REG_DETECTED);
        }
        return 0x00;
}

void charLCD_ST7036::write32(uint8_t reg, uint32_t value) {
	Wire.beginTransmission(_i2cSAddress);
	Wire.write((uint8_t)reg);
	Wire.write(value & 0xFF);
	Wire.write(value >> 8);
     	Wire.write(value >> 16);
	Wire.write(value >> 24);
     	_status = Wire.endTransmission();
}

uint32_t charLCD_ST7036::read32(uint8_t reg) {
	uint32_t val;

	Wire.beginTransmission(_i2cSAddress);
	Wire.write((uint8_t)reg);
	_status = Wire.endTransmission();

	Wire.requestFrom((uint8_t)_i2cSAddress, (uint8_t)4);
		
     	uint8_t buffer[4];
     	for (uint8_t i = 0; i < sizeof(uint32_t); i++){
         buffer[i] = Wire.read();
	}
 	val = (uint32_t)buffer[3]<<24;
	val |= (uint32_t)buffer[2]<<16;
     	val |= (uint32_t)buffer[1]<<8;
	val |= (uint32_t)buffer[0]&0xFF;
   	return val;
}

void charLCD_ST7036::write16(uint8_t reg, uint16_t value) {
	Wire.beginTransmission(_i2cSAddress);
	Wire.write((uint8_t)reg);
	Wire.write(value & 0xFF);
	Wire.write(value >> 8);
     	_status = Wire.endTransmission();
}

uint16_t charLCD_ST7036::read16(uint8_t reg) {
	uint16_t val;

	Wire.beginTransmission(_i2cSAddress);
	Wire.write((uint8_t)reg);
	_status = Wire.endTransmission();

	Wire.requestFrom((uint8_t)_i2cSAddress, (uint8_t)2);
	val = Wire.read();
	val |= Wire.read() <<8;
	return val;
}

void charLCD_ST7036::write8(uint8_t reg, uint8_t value) {
	Wire.beginTransmission(_i2cSAddress);
	Wire.write((uint8_t)reg);
	Wire.write(value);
	_status =  Wire.endTransmission();
}

uint8_t charLCD_ST7036::read8(uint8_t reg) {
	uint8_t val;

	Wire.beginTransmission(_i2cSAddress);
	Wire.write((uint8_t)reg);
	_status = Wire.endTransmission();

	Wire.requestFrom((uint8_t)_i2cSAddress, (uint8_t)1);
	val = Wire.read();
	return val;
}

