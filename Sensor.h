/*
 * Sensor.h
 *
 *  Created on: 3 nov. 2013
 *      Author: Alex
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "Arduino.h"
#include "ArduSensorConf.h"
#include "messages.pb.h"
void sensor_init();
void sensor_retrieve_measure(SensorData* message);
void sensor_start_measure();

//DHT22
#ifdef HAS_DHT22
void DHT22_init();

void DHT22_get_measure(SensorData* o_p_message);
#endif

//BMP085
#ifdef HAS_BMP085
void BMP085_init();
void BMP085_get_measure(SensorData* o_p_message);
#endif

#endif /* SENSOR_H_ */
