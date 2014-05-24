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
#include "pb_decode.h"
#include "EEPROM_ardusensor.h"
#include "ArduSensor.h"
#include "LED.h"

#if MCU == UNO
static RF24 arduSensor_radio(9,10);//SPI on UNO
#endif
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0x7365727631LL };



bool RF_bindRadio(uint64_t i_pipeAddress){

	traceLog("binding w  ");
	traceLogln(i_pipeAddress);

	boolean ret=false;
	boolean timeout=false;
	boolean response_timeout=false;
	boolean status=false;
	int32_t send_time=0;
	int32_t wait_start=0;


	uint8_t test[10];
	for (int i =0;i<10;i++){
		test[i]=0xFF;
	}
	RF_setupRadio();

	arduSensor_radio.openWritingPipe(pipes[0]);
	arduSensor_radio.openReadingPipe(1,pipes[1]);
	arduSensor_radio.stopListening();
	uint8_t PiWeather_SensorDataBuffer[32];
	uint8_t incomingBuffer[32];
	PiWeather_SensorData bindingMess;
	bindingMess.has_type=true;
	bindingMess.has_binding=true;
	bindingMess.binding=true;
	bindingMess.id=eeprom_get_id();/*This means we need an ID*/
	bindingMess.has_battery_level=false;
	bindingMess.type=0;
	bindingMess.has_temperature=false;
	bindingMess.has_pressure=false;
	bindingMess.has_humidity=false;

	pb_ostream_t stream = pb_ostream_from_buffer(PiWeather_SensorDataBuffer, sizeof(PiWeather_SensorDataBuffer));
	pb_encode(&stream, PiWeather_SensorData_fields, &bindingMess);
	//	for (uint8_t i=0;i<stream.bytes_written;i++){
	//Serial.println(PiWeather_SensorDataBuffer[i]);
	//	}
	if(arduSensor_radio.write(PiWeather_SensorDataBuffer,stream.bytes_written))
	{
		arduSensor_radio.startListening();
		traceLog("SEND DONE");
		send_time=millis();
#ifdef DEBUG
		LED_notify_send_done();
#endif
	}
	else
	{
#ifdef DEBUG
		LED_notify_RF_failure();
#endif
	}
	wait_start=millis();
	while(!response_timeout)
	{
		if(arduSensor_radio.available_no_side_effect())
		{
			traceLog("AVAILABLE");
			break;
		}
		else
		{
			if((millis()-wait_start)>INIT_TIMEOUT)
			{
				response_timeout=true;
				traceLogln("TIMEOUT");
			}
		}
	}
	traceLogln("next");
	send_time=millis();
	while ( arduSensor_radio.available() &&!timeout ) {

		uint8_t len = arduSensor_radio.getDynamicPayloadSize();
		arduSensor_radio.read(incomingBuffer , len);
		traceLogln("got ");

		{
#ifdef DEBUG
			LED_notify_error_brief();
#endif
			PiWeather_BindingAck message;
			pb_istream_t stream = pb_istream_from_buffer(incomingBuffer, len);
			status = pb_decode(&stream, PiWeather_BindingAck_fields, &message);

			if (!status)
			{
				traceLogln("Could not parse ack message");
				Serial.print(len);
#ifdef DEBUG
				LED_notify_error_brief();
#endif

			}
			else
			{
				if(eeprom_get_id()==0)
				{
					save_ID_to_EEPROM((uint8_t) message.id);

					traceLog("ID rec ");
					traceLogln((uint8_t) message.id);
					ret=true;
				}
				else if(eeprom_get_id()!=(uint8_t) message.id)
				{
					traceLog("ID NOT CORRECT eeprom: ");
					traceLog(eeprom_get_id());
					traceLog(" received ");
					traceLog(message.id);
					traceLogln(" ");
				}
				else
				{
					ret=true;
				}
				if(message.has_time_interval_s)
				{
					//TODO
					//*o_p_measure_interval=message.time_interval_s;
					traceLogln("Got time interval : ");
					//	traceLogln(*o_p_measure_interval);
				}

			}


		}



		if ( millis() - send_time > INIT_TIMEOUT )
		{
			traceLogln("Timeout!!!");
#ifdef DEBUG
			LED_notify_timeout();
#endif
			timeout = true;
		}
		delay(10);
	}
	arduSensor_radio.stopListening();
	//	delay(200);


	traceLogln("END binding");
	return ret;
}

void RF_radio_sleep(){
	arduSensor_radio.powerDown();
}



void RF_radio_up(){
	arduSensor_radio.powerUp();
}



void RF_send_data(PiWeather_SensorData* message)
{
	uint8_t PiWeather_SensorDataBuffer[32];
	pb_ostream_t stream = pb_ostream_from_buffer(PiWeather_SensorDataBuffer, sizeof(PiWeather_SensorDataBuffer));
	pb_encode(&stream, PiWeather_SensorData_fields, message);
	if(arduSensor_radio.write(PiWeather_SensorDataBuffer,stream.bytes_written)){
		traceLogln("SEND DONE");
#ifdef DEBUG
		LED_notify_send_done();
#endif
	}
	else
	{
#ifdef DEBUG
		LED_notify_timeout();
#endif
		traceLogln("FAIL");
	}
}

void RF_setupRadio(){
	arduSensor_radio.begin();
	arduSensor_radio.enableDynamicPayloads();
	arduSensor_radio.setDataRate(RF24_250KBPS);
	arduSensor_radio.setPALevel(RF24_PA_MAX);
	arduSensor_radio.setChannel(76);
	arduSensor_radio.setRetries(15,15);
	arduSensor_radio.setAutoAck(false);

	arduSensor_radio.printDetails();
	delay(1000);

}
