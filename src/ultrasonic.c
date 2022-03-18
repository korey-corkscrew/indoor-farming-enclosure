#include "ultrasonic.h"

/**
 * @def Sets the GPIO pins as inputs/outputs to interact with the proximity
 *      sensors. Only call this function once during setup.
 */
void ultrasonicInit(void) {
    GPIO_PinOutputEnable(ULTRASONIC_CONTROL_0);
    GPIO_PinInputEnable(ULTRASONIC_ECHO_0);
    GPIO_PinOutputEnable(ULTRASONIC_CONTROL_1);
    GPIO_PinInputEnable(ULTRASONIC_ECHO_1);
}

// -----------------------------------------------------------------------------

/**
 * @def Read the distance in centimeters between a proximity sensor and the light.
 * @param sensorNumber:
 *      0 - Left proximity sensor
 *      1 - Right proximity sensor
 * @return Distance in centimeters
 */
uint8_t readUltrasonicCm(ProximitySensor sensor) {
    uint32_t startCount = 0;
    uint32_t endCount;
    uint8_t centimeters;
    float timeUs;

    switch (sensor) {
            // Left proximity sensor
        case PROXIMITY_SENSOR_LEFT:
            // Send start signal to the proximity sensor
            GPIO_PinClear(ULTRASONIC_CONTROL_0);
            CORETIMER_DelayUs(2);
            GPIO_PinSet(ULTRASONIC_CONTROL_0);
            CORETIMER_DelayUs(10);
            GPIO_PinClear(ULTRASONIC_CONTROL_0);

            // Wait for the signal to toggle high
            while(!GPIO_PinRead(ULTRASONIC_ECHO_0));

            // Start time
            startCount = _CP0_GET_COUNT();

            // Wait for signal to toggle low
            while(GPIO_PinRead(ULTRASONIC_ECHO_0));
            break;

            // Right proximity sensor
        case PROXIMITY_SENSOR_RIGHT:
            // Send start signal to the proximity sensor
            GPIO_PinClear(ULTRASONIC_CONTROL_1);
            CORETIMER_DelayUs(2);
            GPIO_PinSet(ULTRASONIC_CONTROL_1);
            CORETIMER_DelayUs(10);
            GPIO_PinClear(ULTRASONIC_CONTROL_1);

            // Wait for the signal to toggle high
            while(!GPIO_PinRead(ULTRASONIC_ECHO_1));

            // Start time
            startCount = _CP0_GET_COUNT();

            // Wait for signal to toggle low
            while(GPIO_PinRead(ULTRASONIC_ECHO_1));
            break;
    }
    // End time
    endCount = _CP0_GET_COUNT();

    // Calculate time in microseconds
    timeUs = endCount - startCount;

    // Calculate distance between light and sensor in centimeters
    float total = (float) timeUs / 60.0;
    total /= 74.0;
    total /= 2.0;
    total *= 2.54;

    centimeters = (uint8_t) total;

    return centimeters;
}