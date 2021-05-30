#include "driverHWStatus.h"
#include "generalDefines.h"
#include "modEffect.h"

#include "safety_check.h"

#include <stdio.h>

static safety_status_s safety_status; 

static modPowerElectronicsPackStateTypedef *pack_state;

void safety_check_init(modPowerElectronicsPackStateTypedef *copy_pack_state) 
{
    pack_state = copy_pack_state; 
}

void safety_check_task(void) 
{

    safety_status.status_is_ok = (safety_check_cell_over_voltage() & safety_check_cell_under_voltage() 
                                & safety_check_cell_over_temperature() & safety_check_bms_board_over_temperature()
                                & safety_check_power_exceed_80kw()); 

    if (safety_status.status_is_ok) {
        safety_status.bms_fault_data[0] = 0 & 0xFF; 

        #if SAFETY_LED
         modEffectChangeState(STAT_LED_POWER,STAT_RESET);
        #endif 
    } else {
        safety_status.bms_fault_data[0] = 0xFF; 

        #if SAFETY_LED
         modEffectChangeState(STAT_LED_POWER,STAT_SET);
        #endif 
    }
}

bool safety_check_cell_over_voltage(void)
{
    bool cell_is_ok = false; 

    if (pack_state->cellVoltageHigh >= 4.2f) {
        safety_status.bms_fault_data[cell_over_voltage] = 0xFF; 
        cell_is_ok = false; 
    } else {
        safety_status.bms_fault_data[cell_over_voltage] = 0 & 0xFF;
        cell_is_ok = true; 
    }

    return cell_is_ok; 
}

bool safety_check_cell_under_voltage(void)
{
    bool cell_is_ok = false; 

    if (pack_state->cellVoltageLow <= 2.5f) {
        safety_status.bms_fault_data[cell_under_voltage] = 0xFF; 
        cell_is_ok =  false; 
    } else {
        safety_status.bms_fault_data[cell_under_voltage] = 0 & 0xFF;
        cell_is_ok = true; 
    }

    return cell_is_ok;
}

bool safety_check_cell_over_temperature(void)
{
    bool temperature_is_ok = false; 

    if (pack_state->tempBatteryHigh >= 60.0f) {
        safety_status.bms_fault_data[cell_over_temperature] = 0xFF; 
        temperature_is_ok =  false;
    } else {
        safety_status.bms_fault_data[cell_over_temperature] = 0 & 0xFF; 
        temperature_is_ok = true; 
    }

    return temperature_is_ok; 
}

bool safety_check_bms_board_over_temperature(void)
{
    bool bms_temperature_is_ok = false; 

    // It can be bms master board or bms slave boards
    if (pack_state->tempBMSHigh >= 60.0f) {
        safety_status.bms_fault_data[bms_board_over_temperature] = 0xFF; 
        bms_temperature_is_ok = false; 
    } else {
        safety_status.bms_fault_data[bms_board_over_temperature] = 0 & 0xFF; 
        bms_temperature_is_ok = true; 
    }

    return bms_temperature_is_ok; 
}

bool safety_check_power_exceed_80kw(void)
{
    bool power_is_ok = false; 

    float load_power;
    float maximum_allowed_power = 80 * 1000; 
    load_power = pack_state->loCurrentLoadVoltage * pack_state->loCurrentLoadCurrent;
    if (load_power >= maximum_allowed_power) {
        safety_status.bms_fault_data[power_exceed_80kw] = 0xFF; 
        power_is_ok =  false; 
    } else {
        safety_status.bms_fault_data[power_exceed_80kw] = 0 & 0xFF; 
        power_is_ok = true; 
    }
    
    return power_is_ok; 
}

safety_status_s safety_check_get_status(void)
{
    return safety_status; 
}
