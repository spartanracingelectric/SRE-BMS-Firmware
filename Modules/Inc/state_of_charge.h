#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "modPowerElectronics.h"
#include "modConfig.h"

bool state_of_charge_init(modPowerElectronicsPackStateTypedef *copy_pack_state); 
void state_of_charge_task(void); 
static void state_of_charge_calculate(void);
static void state_of_charge_get(void); 
static bool state_of_charge_store_info_to_eeprom(void); 
static bool state_of_charge_get_info_from_eeprom(void);