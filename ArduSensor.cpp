#include "ArduSensor.h"
#include "RF.h"
#include "EEPROM.h"
#include "LED.h"
#include"LowPower.h"
#include "OneWire.h"
#include "Sensor.h"
#include "Trace.h"
#include "EEPROM_ardusensor.h"
#include <avr/sleep.h>



int16_t sleep_counter=-1;
uint8_t pinLED = 2;
static bool binding_ok;
static PiWeather_SensorData message;



void setup()
{
#ifdef DEBUG
//	EEPROM.write(0,0);
#endif
	pinMode(0,INPUT);
	pinMode(LED_R,OUTPUT);
	pinMode(LED_G,OUTPUT);
	pinMode(LED_B,OUTPUT);
	LED_begin();


	digitalWrite(pinLED,HIGH);
	traceInit(COM_SPEED);
	traceLogln("DEBUG");
	traceLogln("");
	traceLogln("");
	if(RF_bindRadio(NETWORK_DATA_PIPE))
	{
		binding_ok=true;
		traceLogln("Binding done");
		sensor_init();
		LED_notify_binding();

	}
	else
	{
		binding_ok=false;
		LED_notify_error();
	}
	RF_radio_sleep();
	message.id=eeprom_get_id();
}


void loop()
{
	if(binding_ok)
	{
		RF_radio_up();
		traceLog("loop ");
		traceLogln(sleep_counter);
		sleep_counter++;
		if(sleep_counter==SLEEPING_COUNTER_MAX-1)
		{
			sensor_start_measure();

			traceLogln("Measure-1");
		}
		else if(sleep_counter==SLEEPING_COUNTER_MAX)
		{
			message.has_battery_level=false;
			message.has_binding=false;
			message.has_humidity=false;
			message.has_pressure=false;
			message.has_temperature=false;
			message.has_type=false;
			sensor_retrieve_measure(&message);
			traceLog(message.temperature);
			RF_send_data(&message);
			sleep_counter=-1;
			traceLogln("Sent data");
		}
		RF_radio_sleep();
	}
	else
	{
		LED_notify_error();
	}

	LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}


const long InternalReferenceVoltage = 1075;  // Adjust this value to your board's specific internal BG voltage

// Code courtesy of "Coding Badly" and "Retrolefty" from the Arduino forum
// results are Vcc * 100
// So for example, 5V would be 500.
uint32_t getBatteryLevel ()
{
	// REFS0 : Selects AVcc external reference
	// MUX3 MUX2 MUX1 : Selects 1.1V (VBG)
	ADMUX = _BV (REFS0) | _BV (MUX3) | _BV (MUX2) | _BV (MUX1);
	ADCSRA |= _BV( ADSC );  // start conversion
	while (ADCSRA & _BV (ADSC))
	{ }  // wait for conversion to complete
	uint32_t results = (((InternalReferenceVoltage * 1024) / ADC) + 5) / 10;
	return 10*results;
} // end of getBandgap


