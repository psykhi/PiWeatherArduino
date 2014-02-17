// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef ArduSensor_H_
#define ArduSensor_H_

#include "Arduino.h"
#include "ArduSensorConf.h"
//LIBRARIES




//PERSO

#include "Trace.h"






//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();

void start_sensor_measurement();

void send_measure();

uint32_t getBatteryLevel();

#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project ArduSensor here




//Do not add code below this line
#endif /* ArduSensor_H_ */
