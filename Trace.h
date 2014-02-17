/*
 * Trace.h
 *
 *  Created on: 21 oct. 2013
 *      Author: Alex
 */

#ifndef TRACE_H_
#define TRACE_H_
#include "ArduSensor.h"


void traceInit(uint32_t i_speed);
void traceLogln(String  i_message);
void traceLogln(uint32_t  i_val);
void traceLog(String  i_message);
void traceLog(uint32_t  i_val);

#endif /* TRACE_H_ */
