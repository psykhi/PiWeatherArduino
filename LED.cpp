/*
 * LED.c
 *
 *  Created on: 29 janv. 2014
 *      Author: Alex
 */
#include "RGBTools.h"
#include "LED.h"
#include "ArduSensorConf.h"



RGBTools status_led(LED_R,LED_G,LED_B);

void LED_set_color(uint8_t i_r,uint8_t i_g, uint8_t i_b)
{
	status_led.setColor(i_r,i_g,i_b);
}


void LED_notify_binding()
{
	status_led.fadeTo(0,255,0,100,500);
	status_led.fadeTo(0,0,0,100,1000);
	status_led.fadeTo(0,255,0,100,1000);
	status_led.fadeTo(0,0,0,100,1000);
	status_led.fadeTo(0,255,0,100,1000);
	status_led.fadeTo(0,0,0,100,1000);
	status_led.fadeTo(0,255,0,100,1000);
	status_led.fadeTo(0,0,0,100,2000);
}

void LED_notify_error()
{
	status_led.fadeTo(255,0,0,100,1000);
	delay(5000);
	status_led.fadeTo(0,0,0,100,1000);
}

void LED_notify_error_brief()
{
	status_led.setColor(255,0,0);
	delay(1);
	status_led.setColor(0,0,0);
}


void LED_notify_low_voltage()
{
	status_led.setColor(239,116,41);
	delay(1);
	status_led.setColor(0,0,0);
}

void LED_begin()
{
	status_led.setColor(0,0,0);
	status_led.fadeTo(255,255,255,100,500);

}
