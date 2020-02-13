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

#define PWMT_I2CADDR_DEFAULT        0x3E
#define PWMT_VALID_ID               0x02

#define PWMT_REG_ID                 0x00
#define PWMT_REG_RESET              0x01
#define PWMT_REG_SAVE               0x02
#define PWMT_REG_ctRatio            0x03
#define PWMT_REG_ctRl               0x05
#define PWMT_REG_trRatio            0x07
#define PWMT_REG_samples            0x0B
#define PWMT_REG_Irms               0x0D
#define PWMT_REG_Vrms               0x11
#define PWMT_REG_power              0x15
#define PWMT_REG_avPower            0x19
#define PWMT_REG_PwFactor           0x1D

class PowerMeter2BRobots {
public:
	typedef union {
		uint32_t i;
		float f;
	}conv;
	PowerMeter2BRobots(uint8_t a = PWMT_I2CADDR_DEFAULT);
	boolean begin();
	uint8_t getID(void);
	void SAVE(void);
	void RESET(void);
	int16_t getCtRatio(void);
	void setCtRatio(int16_t value);
	int16_t getCtRl(void);
	void setCtRl(int16_t value);
	void setTrRatio(float value);
	float getTrRatio(void);
	int16_t getSamples(void);
	void setSamples(int16_t value);
	float getIrms(void);
	float getVrms(void);
	float getPower(void);
	float getAvPower(void);
	float getPowerFactor(void);

	void write32(uint8_t reg, uint32_t val);
	uint32_t read32(uint8_t reg);

	void write16(uint8_t reg, uint16_t val);
	uint16_t read16(uint8_t reg);

	void write8(uint8_t reg, uint8_t val);
	uint8_t read8(uint8_t reg);

private:
	uint8_t _i2caddr = PWMT_I2CADDR_DEFAULT;
};
