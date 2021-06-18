#include "state_of_charge.h"

typedef struct {
	float state_of_charge; // accuraty 0.1 

}; 

static modPowerElectronicsPackStateTypedef *pack_state;

bool state_of_charge_init(modPowerElectronicsPackStateTypedef *copy_pack_state)
{
	if (copy_pack_state != NULL) {
		pack_state = copy_pack_state;
		return true; 
	} else {
		return false; 
	}
}

void state_of_charge_task(void) 
{
	state_of_charge_calculate(); 
	state_of_charge_update(); 
}



/* update the state of charge info based on current reading from shunt 

todo: implement state of charge algorithm 

*/
static float state_of_charge_calculate()
{

}

// update the soc value to main variable 
static float state_of_charge_update()
{
	pack_state->SoC = ; 
	pack_state->SoCCapacityAh = ; 
}

