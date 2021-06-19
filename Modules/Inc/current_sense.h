#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "modPowerElectronics.h"

float current_sense_read_10hz(float current_offset, float shunt_factor); 
