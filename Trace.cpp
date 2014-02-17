/*
 * Trace.cpp
 *
 *  Created on: 21 oct. 2013
 *      Author: Alex
 */

#include "Trace.h"



void traceInit(uint32_t i_speed){
	Serial.begin(i_speed);
}
void traceLogln(String  i_message){
#ifdef DEBUG
	Serial.println(i_message);
#endif
}

void traceLogln(uint32_t  i_val){
#ifdef DEBUG
	Serial.println(i_val);
#endif
}

void traceLog(String  i_message){
#ifdef DEBUG
	Serial.println(i_message);
#endif
}

void traceLog(uint32_t  i_val){
#ifdef DEBUG
	Serial.println(i_val);
#endif
}
