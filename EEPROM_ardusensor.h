/*
 * EEPROM.h
 *
 *  Created on: 5 janv. 2014
 *      Author: Alex
 */

#ifndef EEPROM_H_
#define EEPROM_H_
#include "Arduino.h"


void save_ID_to_EEPROM(uint8_t i_id);

uint8_t eeprom_get_id();

#endif /* EEPROM_H_ */
