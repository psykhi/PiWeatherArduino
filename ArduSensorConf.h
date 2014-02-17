/*
 * ArduSensorConf.h
 *
 *  Created on: 21 oct. 2013
 *      Author: Alex
 */

#ifndef ARDUSENSORCONF_H_
#define ARDUSENSORCONF_H_


#define DEBUG
#define COM_SPEED 115200

/*Pins*/

#define LED_R 3
#define LED_G 5
#define LED_B 6

#define DHT22_PIN 7
#define DHT22_VDD_PIN 2

#define NETWORK_DATA_PIPE 0xF0F0F0F0E1LL //TO move to EEPROM


#define UNO 0
#define MCU UNO

#define SLEEPING_TIME_SECONDS 800 //Time between measurements in seconds
#define SLEEPING_COUNTER_MAX SLEEPING_TIME_SECONDS/8


//#define DS18B20
#define HAS_DHT22
#define HAS_BMP085

#ifdef HAS_DHT22
#define TEMPERATURE
#define HUMIDITY
#endif

#ifdef HAS_BMP085
#define PRESSURE
#endif


#endif /* ARDUSENSORCONF_H_ */
