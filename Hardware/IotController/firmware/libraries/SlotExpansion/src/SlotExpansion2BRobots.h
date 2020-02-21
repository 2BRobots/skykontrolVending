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

#define SLE_I2CADDR_DEFAULT        0x40
#define SLE_VALID_ID               0x03

#define SLE_REG_ID                 0x00
#define SLE_REG_RESET              0x01
#define SLE_REG_SAVE               0x02
#define SLE_REG_dispense           0x03
#define SLE_REG_selected           0x04
#define SLE_REG_cancel             0x05
#define SLE_REG_counter            0x06
#define SLE_REG_time               0x0A
#define SLE_REG_calibrate          0x0E
#define SLE_REG_relay1             0x0F
#define SLE_REG_relay2             0x10
#define SLE_REG_relay3             0x11
#define SLE_REG_relay4             0x12

#define SLE_REG_selected1          0x13
#define SLE_REG_product_id1        0x14
#define SLE_REG_name1              0x21
#define SLE_REG_unit1              0x36
#define SLE_REG_time1              0x39
#define SLE_REG_counter1           0x3D
#define SLE_REG_cost1              0x41
#define SLE_REG_quantity1          0x43
#define SLE_REG_stock1             0x45
#define SLE_REG_capacity1          0x47
#define SLE_REG_emptyLevel1        0x49
#define SLE_REG_fullLevel1         0x4B

#define SLE_REG_selected2          0x4D
#define SLE_REG_product_id2        0x4E
#define SLE_REG_name2              0x5B
#define SLE_REG_unit2              0x70
#define SLE_REG_time2              0x73
#define SLE_REG_counter2           0x77
#define SLE_REG_cost2              0x7B
#define SLE_REG_quantity2          0x7D
#define SLE_REG_stock2             0x7F
#define SLE_REG_capacity2          0x81
#define SLE_REG_emptyLevel2        0x83
#define SLE_REG_fullLevel2         0x85

#define SLE_REG_selected3          0x87
#define SLE_REG_product_id3        0x88
#define SLE_REG_name3              0x95
#define SLE_REG_unit3              0xAA
#define SLE_REG_time3              0xAD
#define SLE_REG_counter3           0xB1
#define SLE_REG_cost3              0xB5
#define SLE_REG_quantity3          0xB7
#define SLE_REG_stock3             0xB9
#define SLE_REG_capacity3          0xBB
#define SLE_REG_emptyLevel3        0xBD
#define SLE_REG_fullLevel3         0xBF

#define SLE_REG_selected4          0xC1
#define SLE_REG_product_id4        0xC2
#define SLE_REG_name4              0xCF
#define SLE_REG_unit4              0xE4
#define SLE_REG_time4              0xE7
#define SLE_REG_counter4           0xEB
#define SLE_REG_cost4              0xEF
#define SLE_REG_quantity4          0xF1
#define SLE_REG_stock4             0xF3
#define SLE_REG_capacity4          0xF5
#define SLE_REG_emptyLevel4        0xF7
#define SLE_REG_fullLevel4         0xF9

class SlotExpansion2BRobots {
public:
	typedef union {
		uint32_t i;
		float f;
	}conv;
	SlotExpansion2BRobots(uint8_t a = SLE_I2CADDR_DEFAULT);
	boolean begin();
	uint8_t getID(void);
	void SAVE(void);
	void RESET(void);
	void startDispensing(void);
	void stopDispensing(void);
	bool dispensingDone(void);
	uint8_t getSelected(void);
	void setSelected(uint8_t value);
	uint8_t getCancel(void);
	void setCancel(uint8_t value);
	uint32_t getCount(void);
	void setCount(uint32_t value);
	uint32_t getTimer(void);
	void setTimer(uint32_t value);
	uint8_t getCalibrate(void);
	void setCalibrate(uint8_t value);
	uint8_t getRelay(uint8_t slot);
	void setRelay(uint8_t slot, uint8_t value);

	uint8_t isSelected(uint8_t slot);
	void changeSelected(uint8_t slot, uint8_t value);
	String getProductID(uint8_t slot);
	void setProductID(uint8_t slot, String value);
	String getName(uint8_t slot);
	void setName(uint8_t slot, String value);
	String getUnit(uint8_t slot);
	void setUnit(uint8_t slot, String value);
	uint32_t getTime(uint8_t slot);
	void setTime(uint8_t slot, uint32_t value);
	uint32_t getCounter(uint8_t slot);
	void setCounter(uint8_t slot, uint32_t value);
	uint16_t getCost(uint8_t slot);
	void setCost(uint8_t slot, uint16_t value);
	uint16_t getQuantity(uint8_t slot);
	void setQuantity(uint8_t slot, uint16_t value);
	uint16_t getStock(uint8_t slot);
	void setStock(uint8_t slot, uint16_t value);
	uint16_t getCapacity(uint8_t slot);
	void setCapacity(uint8_t slot, uint16_t value);
	uint16_t getEmptyLevel(uint8_t slot);
	void setEmptyLevel(uint8_t slot, uint16_t value);
	uint16_t getFullLevel(uint8_t slot);
	void setFullLevel(uint8_t slot, uint16_t value);

	void write32(uint8_t reg, uint32_t val);
	uint32_t read32(uint8_t reg);

	void write16(uint8_t reg, uint16_t val);
	uint16_t read16(uint8_t reg);

	void write8(uint8_t reg, uint8_t val);
	uint8_t read8(uint8_t reg);

private:
	uint8_t _i2caddr = SLE_I2CADDR_DEFAULT;
};
