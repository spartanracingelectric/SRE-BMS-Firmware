#include "driverSWSSD1306.h"
#include "modDelay.h"
#include "libGraphics.h"
#include "libLogos.h"
#include "driverHWPowerState.h"

#define REFRESHTIMOUT	30																		// Interval between complete sync between desired and actual display content					
#define STARTUPDELAY	10																		// Startup delay, wait this amount of time before talking to the display

#ifndef MODDISPLAY_H_
#define MODDISPLAY_H_

typedef struct {
	float StateOfCharge;
	float Current;
	float PackVoltage;
	float LoadVoltage;
	float HighestTemp;
	float AverageTemp;
	float LowestTemp;
	float LowestCellVoltage;
	float HighestCellVoltage;
	float AverageCellVoltage;
	float Humidity;
	uint8_t DisplayStyle;
	uint8_t FaultCode;
	float CellMismatch;
	float ChargerVoltage;
} modDisplayDataTypedef;

typedef enum {
	DISP_MODE_OFF = 0,
	DISP_MODE_SPLASH,	
	DISP_MODE_LOAD,
	DISP_MODE_CHARGE,
	DISP_MODE_POWEROFF,
	DISP_MODE_ERROR,
	DISP_MODE_ERROR_PRECHARGE,
	DISP_MODE_EXTERNAL,
	DISP_MODE_BATTERY_DEAD,
	DISP_MODE_BALANCING,
	DISP_MODE_CHARGED,
	DISP_MODE_FORCED_ON
} modDisplayInfoType;

void modDisplayInit(void);
void modDisplayShowInfo(modDisplayInfoType newState,modDisplayDataTypedef modDisplayData);
void modDisplayTask(void);

void modDisplayWrite(float value, uint8_t decimals);
uint8_t modDisplay100ConvertValueToASCII(float value);
uint8_t modDisplay10ConvertValueToASCII(float value);
uint8_t modDisplay1ConvertValueToASCII(float value);
uint8_t modDisplay0_1ConvertValueToASCII(float value);
uint8_t modDisplay0_01ConvertValueToASCII(float value);
uint8_t modDisplay0_001ConvertValueToASCII(float value);
#endif
