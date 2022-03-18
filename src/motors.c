#include "motors.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 * @def
 */
void motorInit( void ) {
    GPIO_PinOutputEnable( MOTOR_CONTROL_EN );
    GPIO_PinOutputEnable( MOTOR_0_IN1 ); 
    GPIO_PinOutputEnable( MOTOR_0_IN2 ); 
    GPIO_PinOutputEnable( MOTOR_1_IN1 ); 
    GPIO_PinOutputEnable( MOTOR_1_IN2 );
}

// -----------------------------------------------------------------------------

/**
 * @def
 * @param motor
 * @param direction
 * @param ms
 */
void moveMotors(Direction direction, uint32_t ms) {
    // Raise lights
    if (direction == UP) {
        GPIO_PinSet(MOTOR_0_IN1);
        GPIO_PinClear(MOTOR_0_IN2);
        GPIO_PinSet(MOTOR_1_IN1);
        GPIO_PinClear(MOTOR_1_IN2);
        GPIO_PinSet(MOTOR_CONTROL_EN);

    }                
    // Lower lights
    else {
        GPIO_PinClear(MOTOR_0_IN1);
        GPIO_PinSet(MOTOR_0_IN2);
        GPIO_PinClear(MOTOR_1_IN1);
        GPIO_PinSet(MOTOR_1_IN2);
        GPIO_PinSet(MOTOR_CONTROL_EN);
    }
    
    CORETIMER_DelayMs( ms );
    GPIO_PinClear(MOTOR_CONTROL_EN);
}

void moveMotor( Motor motor, Direction direction, uint32_t ms) {
    switch( motor ) {
        case MOTOR_0:
            // Raise lights
            if (direction == UP) {
                // Set motor 0 to move forward 
                GPIO_PinSet(MOTOR_0_IN1);
                GPIO_PinClear(MOTOR_0_IN2);
            }                
            // Lower lights
            else {
                // Set motor 0 to move in reverse
                GPIO_PinClear(MOTOR_0_IN1);
                GPIO_PinSet(MOTOR_0_IN2);
            }
            
            // Turn off motor 1
            GPIO_PinClear(MOTOR_1_IN1);
            GPIO_PinClear(MOTOR_1_IN2);
            break;
        case MOTOR_1:
            // Raise lights
            if (direction == UP) {
                // Set motor 1 to move forward 
                GPIO_PinSet(MOTOR_1_IN1);
                GPIO_PinClear(MOTOR_1_IN2);
            }                
            // Lower lights
            else {
                // Set motor 1 to move in reverse
                GPIO_PinClear(MOTOR_1_IN1);
                GPIO_PinSet(MOTOR_1_IN2);
            }
            
            // Turn off motor 0
            GPIO_PinClear(MOTOR_0_IN1);
            GPIO_PinClear(MOTOR_0_IN2);
            break;
    }
    
    // Enable motor driver
    GPIO_PinSet(MOTOR_CONTROL_EN);
    
    // Keep motor on for desired amount of time
    CORETIMER_DelayMs( ms );
    
    // Turn off motor driver
    GPIO_PinClear(MOTOR_CONTROL_EN);
}
