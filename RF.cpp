/*
 * RF.cpp
 *
 *  Created on: 21 oct. 2013
 *      Author: Alex
 */


#include "RF.h"
#include "Trace.h"
#include "messages.pb.h"
#include "pb_encode.h"
#include "EEPROM_ardusensor.h"
#include "ArduSensor.h"

#if MCU == UNO
static RF24 arduSensor_radio(9,10);//SPI on UNO
#endif
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0x7365727631LL };
static uint8_t sensor_ID=0;


bool RF_bindRadio(uint64_t i_pipeAddress){

	traceLog("NOW TRYING TO BIND WITH PIPE ");
	traceLogln(i_pipeAddress);

	boolean ret=false;
	boolean timeout=false;
	int32_t send_time=0;


	uint8_t test[10];
	for (int i =0;i<10;i++){
		test[i]=0xFF;
	}
	RF_setupRadio();

	arduSensor_radio.openWritingPipe(pipes[0]);
	arduSensor_radio.openReadingPipe(1,pipes[1]);
	arduSensor_radio.stopListening();
	uint8_t sensorDataBuffer[32];
	uint8_t incomingBuffer[32];
	SensorData bindingMess;
	bindingMess.has_type=true;
	bindingMess.has_binding=true;
	bindingMess.binding=true;
	bindingMess.id=eeprom_get_id();/*This means we need an ID*/
	bindingMess.has_battery_level=true;
	bindingMess.battery_level=getBatteryLevel();
	bindingMess.type=0;
	bindingMess.has_temperature=false;
	bindingMess.has_pressure=false;

	pb_ostream_t stream = pb_ostream_from_buffer(sensorDataBuffer, sizeof(sensorDataBuffer));
	pb_encode(&stream, SensorData_fields, &bindingMess);
	//	for (uint8_t i=0;i<stream.bytes_written;i++){
	//Serial.println(sensorDataBuffer[i]);
	//	}
	if(arduSensor_radio.write(sensorDataBuffer,stream.bytes_written)){
		traceLog("SEND DONE");
		send_time=millis();
	}

	arduSensor_radio.startListening();
	delay(2000);

	while ( arduSensor_radio.available() &&!timeout ) {

		uint8_t len = arduSensor_radio.getDynamicPayloadSize();
		arduSensor_radio.read(incomingBuffer , len);
		traceLogln("got something");
		if(eeprom_get_id()==0)
		{
			save_ID_to_EEPROM(incomingBuffer[0]);
			traceLog("ID received ");
			traceLogln(incomingBuffer[0]);
			ret=true;
		}
		else if(incomingBuffer[0]!=eeprom_get_id()){
			traceLog("ID RECEIVED NOT CORRECT");

		}
		else
		{
			traceLog("Got ACK");
			ret=true;
		}

		if ( millis() - send_time > arduSensor_radio.getMaxTimeout() ) {
			traceLogln("Timeout!!!");
			timeout = true;
		}
		delay(10);
	}
	arduSensor_radio.stopListening();
	//	delay(200);


	traceLogln("END of radio binding");
	return ret;
}

void RF_radio_sleep(){
	arduSensor_radio.powerDown();
}



void RF_radio_up(){
	arduSensor_radio.powerUp();
}



void RF_send_data(SensorData* message)
{
	uint8_t sensorDataBuffer[32];
	pb_ostream_t stream = pb_ostream_from_buffer(sensorDataBuffer, sizeof(sensorDataBuffer));
	pb_encode(&stream, SensorData_fields, message);
	if(arduSensor_radio.write(sensorDataBuffer,stream.bytes_written)){
		traceLog("SEND DONE");
	}
}
void RF_setupRadio(){
	arduSensor_radio.begin();
	arduSensor_radio.enableDynamicPayloads();
	arduSensor_radio.setDataRate(RF24_1MBPS);
	arduSensor_radio.setPALevel(RF24_PA_MAX);
	arduSensor_radio.setChannel(76);
	arduSensor_radio.setRetries(15,15);

	arduSensor_radio.printDetails();
	delay(1000);

}
