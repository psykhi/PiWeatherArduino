/*
 * LED.h
 *
 *  Created on: 29 janv. 2014
 *      Author: Alex
 */

#ifndef LED_H_
#define LED_H_

void LED_set_color(uint8_t i_r,uint8_t i_g, uint8_t i_b);
void LED_notify_binding();
void LED_notify_error();
void LED_notify_error_brief();
void LED_notify_low_voltage();
void LED_begin();
void LED_notify_timeout();
void LED_notify_send_done();
void LED_notify_RF_failure();
#endif /* LED_H_ */
