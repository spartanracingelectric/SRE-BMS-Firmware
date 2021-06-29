#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "modPowerElectronics.h"

void current_sense_init(modPowerElectronicsPackStateTypedef *copy_pack_state); 
void current_sense_read_5hz(float current_offset, float shunt_factor);
