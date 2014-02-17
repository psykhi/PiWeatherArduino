/*
 * Sensor.cpp
 *
 *  Created on: 3 nov. 2013
 *      Author: Alex
 */

#include "Sensor.h"

#ifdef HAS_DHT22
#include "DHT22.h"
#endif
#ifdef DS18B20
#include "OneWire.h"
#endif

#ifdef HAS_BMP085
#include "Adafruit_BMP085.h"
#include "Wire.h"
#endif
#include "Trace.h"
#include "RF.h"
#include "EEPROM_ardusensor.h"
#include "LED.h"

#ifdef DS18B20
OneWire  ds(DS18B20_DATA_PIN);
static byte addr[8];
#endif

#ifdef HAS_DHT22
DHT22 DHT_22(DHT22_PIN);
#endif

#ifdef HAS_BMP085
Adafruit_BMP085 bmp;
#endif

void sensor_init()
{



	traceLog("Init the sensor");


#ifdef HAS_DHT22
	DHT22_init();
#endif
#ifdef HAS_BMP085
	BMP085_init();
#endif
}

void sensor_start_measure(){
#ifdef DS18B20
	DS18B20_start_measure();
#endif

}

#ifdef DS18B20
void DS18B20_start_measure()
{
	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1);        // start conversion, with parasite power on at the end

}
#endif
#ifdef DS18B20
void DS18B20_get_measure(byte* data)
{
	byte present = 0;
	byte i=0;


	present = ds.reset();
	ds.select(addr);
	ds.write(0xBE);         // Read Scratchpad
#ifdef DEBUG
	Serial.print("  Data = ");
	Serial.print(present, HEX);
	Serial.print(" ");
#endif
	for ( i = 0; i < 9; i++) {           // we need 9 bytes
		data[i] = ds.read();
#ifdef DEBUG
		Serial.print(data[i], HEX);
		Serial.print(" ");
#endif
	}
#ifdef DEBUG
	Serial.print(" CRC=");
	Serial.print(OneWire::crc8(data, 8), HEX);
	Serial.println();
#endif
}
#endif

void sensor_retrieve_measure(SensorData* o_p_message)
{


	o_p_message->id=eeprom_get_id();

#ifdef DS18B20
	DS18B20_get_measure();
#endif
#ifdef HAS_DHT22
	DHT22_get_measure(o_p_message);
#endif
#ifdef HAS_BMP085
	BMP085_get_measure(o_p_message);
#endif
}


#ifdef HAS_DHT22
void DHT22_init()
{
	pinMode(DHT22_VDD_PIN,OUTPUT);
	digitalWrite(DHT22_VDD_PIN,LOW);
}


void DHT22_get_measure(SensorData* message)
{
	digitalWrite(DHT22_VDD_PIN,HIGH);
	delay(2500);
	DHT22_ERROR_t errorCode=DHT_ERROR_NOT_PRESENT;
	errorCode = DHT_22.readData();
	digitalWrite(DHT22_VDD_PIN,LOW);

	Serial.print("error code ");
	Serial.println(errorCode);
	switch(errorCode)
	{
	case DHT_ERROR_NONE:
		message->has_temperature=true;
		message->temperature=DHT_22.getTemperatureC();
		message->humidity=DHT_22.getHumidity();
		message->has_humidity=true;
		break;
	default:
		LED_notify_error_brief();
		break;
	}

}

#endif
#ifdef HAS_BMP085
void BMP085_init()
{

	bmp.begin(BMP085_ULTRALOWPOWER);

}
void BMP085_get_measure(SensorData* o_p_message)
{


	o_p_message->has_pressure=true;
	o_p_message->pressure=((float)(bmp.readPressure()))/100;

}
#endif


#ifdef DS18B20
void DS18B20_init()
{
	byte i;
	if ( !ds.search(addr)) {
		traceLogln("No more addresses.");
		traceLogln(" ");
		ds.reset_search();
		delay(250);
		return;
	}
#ifdef DEBUG
	Serial.print("ROM =");
	for( i = 0; i < 8; i++) {
		Serial.write(' ');
		Serial.print(addr[i], HEX);
	}
#endif
}

float decode_DS18B20(byte* data)
{
	float celsius=0;
	int16_t raw = (data[1] << 8) | data[0];

	byte cfg = (data[4] & 0x60);
	// at lower res, the low bits are undefined, so let's zero them
	if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
	else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
	else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
	//// default is 12 bit resolution, 750 ms conversion time

	celsius = (float)raw / 16.0;
	return celsius;
}

#endif
