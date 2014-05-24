/*
 * EEPROM.cpp
 *
 *  Created on: 5 janv. 2014
 *      Author: Alex
 */


#include "EEPROM_ardusensor.h"
#include "EEPROM.h"
#include "Trace.h"

void save_ID_to_EEPROM(uint8_t i_id){
	traceLog("Saving ID");
	EEPROM.write(0,i_id);
}


uint8_t eeprom_get_id()
{
return EEPROM.read(0);
}
