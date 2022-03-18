/* 
 * File:   relays.h
 * Author: cbini
 *
 * Created on March 11, 2022, 7:40 PM
 */

#ifndef RELAYS_H
#define	RELAYS_H

#include "definitions.h"                // SYS function prototypes

#ifdef	__cplusplus
extern "C" {
#endif

// Relay states
typedef enum {
    ON = true,
    OFF = false
} State;

// Relays
typedef enum {
    HEATER = GPIO_PIN_RA10,
    FAN_IN = GPIO_PIN_RB13,
    FAN_OUT = GPIO_PIN_RB12,
    HUMIDIFIER = GPIO_PIN_RB11,
    LIGHTS = GPIO_PIN_RB10,
    RELAY_5 = GPIO_PIN_RF1,
    RELAY_6 = GPIO_PIN_RF0,
    RELAY_7 = GPIO_PIN_RD6
} Relay;

void relaysInit(void);
void controlRelay(Relay relay, State state);

#ifdef	__cplusplus
}
#endif

#endif	/* RELAYS_H */

