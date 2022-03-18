/* 
 * File:   ultrasonic.h
 * Author: cbini
 *
 * Created on March 11, 2022, 7:46 PM
 */

#ifndef ULTRASONIC_H
#define	ULTRASONIC_H

#include "definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define ULTRASONIC_CONTROL_0    GPIO_PIN_RA7
#define ULTRASONIC_ECHO_0       GPIO_PIN_RB14
#define ULTRASONIC_CONTROL_1    GPIO_PIN_RD5
#define ULTRASONIC_ECHO_1       GPIO_PIN_RD8
    
typedef enum {
    PROXIMITY_SENSOR_LEFT,
    PROXIMITY_SENSOR_RIGHT
} ProximitySensor;

void ultrasonicInit(void);
uint8_t readUltrasonicCm(ProximitySensor sensor);


#ifdef	__cplusplus
}
#endif

#endif	/* ULTRASONIC_H */

