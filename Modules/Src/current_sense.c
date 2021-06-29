#include "current_sense.h"

#include "stm32f3xx_hal.h"

#include "driverSWISL28022.h"

static float load_current_last_reading; 
static float load_current;
static uint32_t timestamp_5hz;
static uint8_t current_sense_counter; 

static modPowerElectronicsPackStateTypedef *pack_state;

void current_sense_init(modPowerElectronicsPackStateTypedef *copy_pack_state)
{
    timestamp_5hz = HAL_GetTick(); 
    current_sense_counter = 0;
    pack_state = copy_pack_state;
}

void current_sense_read_5hz(float current_offset, float shunt_factor)
{
    bool is_current_data_valid = false;

    static float load_current_temp;

    driverSWISL28022GetBusCurrent(ISL28022_MASTER_ADDRES,ISL28022_MASTER_BUS, &load_current_temp, current_offset, shunt_factor);

    if ((abs(load_current_temp - load_current_last_reading) < 0.1) | (abs(load_current_temp - load_current_last_reading) > 270)) {
        is_current_data_valid = false; 
    } else {
        is_current_data_valid  = true; 
    }

    load_current_last_reading = load_current_temp;

    if ((HAL_GetTick() - timestamp_5hz) > 200) {
        pack_state->loCurrentLoadCurrent = load_current_temp;
        timestamp_5hz = HAL_GetTick(); 
    }

    // if (0 == current_sense_counter) {
    //     pack_state->loCurrentLoadVoltage = load_current_temp; 
    // } else {
    //     if ((HAL_GetTick() - timestamp_5hz) < 200) {
    //         if (is_current_data_valid) {
    //             load_current += load_current_temp; 
    //             current_sense_counter++; 
    //         }
    //     } else {
    //         pack_state->loCurrentLoadVoltage = load_current / current_sense_counter; 
    //         load_current = 0; 
    //         current_sense_counter = 0; 
    //     }
    // }
}
