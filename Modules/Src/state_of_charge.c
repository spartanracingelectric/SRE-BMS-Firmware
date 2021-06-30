#include "state_of_charge.h"
#include "state_of_health.h"

#include "driverSWStorageManager.h"
#include "modStateOfCharge.h"

modStateOfChargeStructTypeDef soc; 

uint32_t soc_last_stored_time_ms;  

static modPowerElectronicsPackStateTypedef *pack_state;

bool state_of_charge_init(modPowerElectronicsPackStateTypedef *copy_pack_state)
{
	if (copy_pack_state == NULL) {
		return false; 
	} else {
		pack_state = copy_pack_state;
	}

	driverSWStorageManagerStateOfChargeStructSize = (sizeof(modStateOfChargeStructTypeDef) / sizeof(uint16_t));

	soc_last_stored_time_ms = HAL_GetTick();

	return true;
}

void state_of_charge_task(void) 
{
	state_of_charge_calculate(); 
	state_of_charge_get();  
}

/* update the state of charge info based on current reading from shunt 

we want soc automatically cover both discharge and charge

discharge has a negative current reading

chare has a positive current reading

todo: implement state of charge algorithm 

*/
static void state_of_charge_calculate(void)
{
	// state_of_charge_read_from_eeprom(); will give the current soc status
	static modStateOfChargeStructTypeDef current_soc;

	state_of_charge_get_info_from_eeprom();

	current_soc = soc; 

	// dt ticks is in ms

	uint32_t dt = HAL_GetTick() - soc_last_stored_time_ms; 

	current_soc.remainingCapacityAh += (dt * pack_state->loCurrentLoadCurrent) / (60 * 60 * 1000);

	if (current_soc.remainingCapacityAh > 27.0) {
		current_soc.remainingCapacityAh = 27; 
	}

	if (current_soc.remainingCapacityAh < 0.0) {
		current_soc.remainingCapacityAh = 0; 
	}

	// general state of charge is in percent
	current_soc.generalStateOfCharge = (soc.remainingCapacityAh / 27) * 100; 

	soc = current_soc; 

	state_of_charge_store_info_to_eeprom(); 
}

// update the soc value to main variable 
static void state_of_charge_get(void)
{
	if (state_of_charge_get_info_from_eeprom()) {
		pack_state->SoC = soc.generalStateOfCharge; 
		pack_state->SoCCapacityAh = soc.remainingCapacityAh;
	}
	// need to add a error handling here
}

static bool state_of_charge_store_info_to_eeprom(void)
{
	bool store_success; 
	store_success = driverSWStorageManagerStoreStruct(&soc, STORAGE_STATEOFCHARGE);
	return store_success; 
}

static bool state_of_charge_get_info_from_eeprom(void)
{
	bool read_success; 
	read_success = driverSWStorageManagerGetStruct(&soc, STORAGE_STATEOFCHARGE);
	return read_success; 
}





