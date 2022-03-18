#include "relays.h"

/**
 * @def Set the GPIO pins connected to the relays as outputs to control the relay
 *      state. Only call this function once during setup.
 */
void relaysInit(void) {
    GPIO_PinOutputEnable(HEATER);
    GPIO_PinOutputEnable(FAN_IN);
    GPIO_PinOutputEnable(FAN_OUT);
    GPIO_PinOutputEnable(HUMIDIFIER);
    GPIO_PinOutputEnable(LIGHTS);
    GPIO_PinOutputEnable(RELAY_5);
    GPIO_PinOutputEnable(RELAY_6);
    GPIO_PinOutputEnable(RELAY_7);
}

/**
 * @def Controls the state of a given relay.
 * @param relay:
 *      Relay to control (i.e. HEATER)
 * @param state:
 *      State of the relay (ON or OFF)
 */
void controlRelay(Relay relay, State state) {
    // Turn relay on
    if (state) {
        GPIO_PinSet(relay);
    }        // Turn relay off
    else {
        GPIO_PinClear(relay);
    }
}