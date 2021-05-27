#ifndef __DRIVERSWINA238_H
#define __DRIVERSWINA238_H

#include "driverHWI2C2.h"
#include "driverHWI2C1.h"

#define INA238_MASTER_ADDRES      0x45
#define INA238_MASTER_BUS         2

typedef enum {
	BRNG_16V = 0,
	BRNG_32V,
	BRNG_60V_0,																			// Both 10 and 11 set range to 60V
	BRNG_60V_1,																			// Both 10 and 11 set range to 60V
} driverSWISL28022BRNGConfigRegister;

typedef enum {
	PGA_1_40MV = 0,																	// Devide by 1, range +/- 40mV
	PGA_2_80MV,																			// Devide by 2, range +/- 80mV
	PGA_4_160MV,																		// Devide by 4, range +/- 160mV
	PGA_8_320MV,																		// Devide by 8, range +/- 320mV
} driverSWISL28022PGAConfigRegister;

typedef enum {
	ADC_12BIT_72US = 0x00,
	ADC_13BIT_132US = 0x01,
	ADC_14BIT_258US = 0x02,
	ADC_15BIT_0_508US = 0x03,												// Both _0 and _1 have same result
	ADC_15BIT_1_508US = 0x08,
	ADC_2_1010US = 0x09,
	ADC_4_2010US = 0x0A,
	ADC_8_4010US = 0x0B,
	ADC_16_8010US = 0x0C,
	ADC_32_16010US = 0x0D,
	ADC_64_32010US = 0x0E,	
	ADC_128_64010US = 0x0F	
} driverSWISL28022ADCConfigRegister;

typedef enum {
	MODE_POWERDOWN = 0,
	MODE_SHUNTVOLTAGE_TRIGGERED,
	MODE_BUSVOLTAGE_TRIGGERED,
	MODE_SHUNTANDBUS_TRIGGERED,
	MODE_ADC_OFF,
	MODE_SHUNTVOLTAGE_CONTINIOUS,
	MODE_BUSVOLTAGE_CONTINIOUS,
	MODE_SHUNTANDBUS_CONTINIOUS
} driverSWISL28022ModeConfigRegister;

typedef enum {
	REG_CONFIG = 0,
	REG_ADCCONFIG,
	REG_CURRLSBCALC,
	REG_VSHUNT,
	REG_VBUS,
	REG_DIETEMP,
	REG_CURRENT,
	REG_POWER,
	REG_DIAG_ALERT = 11,
	REG_SOVL,
	REG_SUVL,
	REG_BOVL,
	REG_BUVL,
	REG_TEMP_LIMIT,
	REG_PWR_LIMIT,
	REG_MANUFACTURER_ID = 62,
	REG_DEVICE_ID
} driverSWISL28022Registers;

typedef struct {
	driverSWISL28022BRNGConfigRegister busVoltageRange;
	uint8_t currentShuntGain;
	uint8_t ADCSetting;
	uint8_t Mode;
} driverSWISL28022InitStruct;

void driverSWINA238Init(uint8_t i2cAddres, uint8_t i2cBus, driverSWISL28022InitStruct initStruct);
bool driverSWINA238GetBusCurrent(uint8_t i2cAddres, uint8_t i2cBus, float *busCurrent, int16_t offset, float scalar);
bool driverSWINA238GetBusVoltage(uint8_t i2cAddres, uint8_t i2cBus, float *busVoltage, int16_t offset, float scalar);

#endif
