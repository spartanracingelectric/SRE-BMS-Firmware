#include "current_sense.h"

#include "driverSWISL28022.h"

static float load_current_last_reading; 
static float load_current; 

float current_sense_read_10hz(float current_offset, float shunt_factor)
{
    bool is_current_data_valid = false; 

    load_current_last_reading = load_current; 

    driverSWISL28022GetBusCurrent(ISL28022_MASTER_ADDRES,ISL28022_MASTER_BUS, &load_current, current_offset, shunt_factor);

    if (abs(load_current - load_current_last_reading) <= 0.5 || abs(load_current - load_current_last_reading) >= 300) {
        is_current_data_valid = false; 
    } else {
        is_current_data_valid = true; 
    }

    if (!is_current_data_valid) {
        load_current = load_current_last_reading; 
    }

    return load_current; 
}