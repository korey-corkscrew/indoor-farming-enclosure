/* 
 * File:   motors.h
 * Author: cbini
 *
 * Created on March 11, 2022, 7:54 PM
 */

#ifndef MOTORS_H
#define	MOTORS_H

#include "definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif

// Motor control enable pin
#define MOTOR_CONTROL_EN        GPIO_PIN_RB9
     
#define MOTOR_0_IN1             GPIO_PIN_RC6
#define MOTOR_0_IN2             GPIO_PIN_RC7
#define MOTOR_1_IN1             GPIO_PIN_RC8
#define MOTOR_1_IN2             GPIO_PIN_RC9

// Motors
typedef enum {
    MOTOR_0,
    MOTOR_1
} Motor;

// Directions
typedef enum {
    UP,
    DOWN
} Direction;

// Functions
void motorInit( void );
void moveMotors(Direction direction, uint32_t ms);
void moveMotor( Motor motor, Direction direction, uint32_t ms);

#ifdef	__cplusplus
}
#endif

#endif	/* MOTORS_H */

