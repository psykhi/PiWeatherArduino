/*
 * Binding.h
 *
 *  Created on: 21 oct. 2013
 *      Author: Alex
 */

#ifndef RF_H_
#define RF_H_
#include "RF24.h"
#include "ArduSensorConf.h"
#include "messages.pb.h"

/*********RADIO*********/

const uint64_t pipe = NETWORK_DATA_PIPE;



bool RF_bindRadio(uint64_t i_pipeAddress);
void RF_setupRadio();
void RF_radio_sleep();
void RF_radio_up();

void RF_send_data(PiWeather_SensorData* i_p_message);

float RF_decode_DS18B20(byte* data);

#endif /* BINDING_H_ */
