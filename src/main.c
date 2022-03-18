#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "TSL2591.h"                    // Luminosity sensors
#include "Si7021.h"                     // Temperature & humidity sensors
#include "relays.h"                     // 5VDC / 110VAC relays
#include "ultrasonic.h"                 // Ultrasonic proximity sensors
#include "motors.h"                     // Brushed DC motors
#include "LiquidCrystalI2C.h"           // 16 x 2 LCD


// Desired conditions sent from control panel
uint8_t desiredTemperature;
uint8_t desiredHumidity;
uint16_t desiredLuminosity;

// Current conditions of the enclosure
uint8_t currentTemperature;
uint8_t currentHumidity;
uint16_t currentLuminosity;

// Reserve buffer space for data sent between the enclosure and control panel
uint8_t incomingData[4];
uint8_t incomingDataCounter;
uint8_t outgoingData[4];
uint8_t outgoingDataCounter;


// -----------------------------------------------------------------------------
// Prototypes
// -----------------------------------------------------------------------------
bool APP_I2C_SLAVE_Callback(I2C_SLAVE_TRANSFER_EVENT event, uintptr_t contextHandle);
void setup(void);
void climateControl(void);

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 * @def Handles I2C slave interrupt from master. Reads the desired conditions
 *      sent from the master and sends the current conditions to the master for
 *      data storage.
 * 
 *      Control Panel --> Enclosure
 *      Master --> | byte 1 | byte 2 | byte 3 | byte 4 | --> Slave
 *          Byte 1: Desired temperature
 *          Byte 2: Desired humidity
 *          Byte 3: Desired luminosity high byte
 *          Byte 4: Desired luminosity low byte
 * 
 *      Enclosure --> Control Panel
 *      Slave --> | byte 1 | byte 2 | byte 3 | byte 4 | --> Master
 *          Byte 1: Current temperature
 *          Byte 2: Current humidity
 *          Byte 3: Current luminosity high byte
 *          Byte 4: Current luminosity low byte
 * 
 * @param event:
 *      I2C slave interrupt event
 * @param contextHandle:
 *      Always pass '0'
 * @return Successful transmission
 */
bool APP_I2C_SLAVE_Callback(I2C_SLAVE_TRANSFER_EVENT event, uintptr_t contextHandle) {
    switch (event) {
            // I2C master is sending data to this device
        case I2C_SLAVE_TRANSFER_EVENT_ADDR_MATCH:
            // Reset counters
            outgoingDataCounter = 0;
            incomingDataCounter = 0;

            // Load outgoing data buffer with current conditions
            outgoingData[0] = readTemperature(TEMP_HUM_SENSOR_0);
            outgoingData[1] = readHumidity(TEMP_HUM_SENSOR_0);
            uint16_t currentLuminosity = readLuminosity(LUM_SENSOR_0, TSL2591_FULLSPECTRUM);
            outgoingData[2] = (uint8_t) (currentLuminosity >> 8);
            outgoingData[3] = (uint8_t) (currentLuminosity & 0x00FF);
            break;

            // I2C master is sending the desired conditions
        case I2C_SLAVE_TRANSFER_EVENT_RX_READY:
            // Read desired conditions sent by the control panel
            while (I2C2_IsBusy());
            incomingData[incomingDataCounter++] = I2C2_ReadByte();
            while (I2C2_IsBusy());
            break;

            // I2C master is ready to receive data from slave
        case I2C_SLAVE_TRANSFER_EVENT_TX_READY:
            // Send current conditions to control panel
            while (I2C2_IsBusy());
            I2C2_WriteByte(outgoingData[outgoingDataCounter++]);
            while (I2C2_IsBusy());
            break;

            // I2C master is finished writing data
        case I2C_SLAVE_TRANSFER_EVENT_STOP_BIT_RECEIVED:
            // Set desired conditions from incoming data buffer
            desiredTemperature = incomingData[0];
            desiredHumidity = incomingData[1];
            desiredLuminosity = incomingData[2];
            desiredLuminosity = (desiredLuminosity << 8) | incomingData[3];
            break;

        default:
            break;
    }

    return true;
}

// -----------------------------------------------------------------------------

void climateControl(void) {
    // Get current conditions of the enclosure
    currentTemperature = readTemperature(TEMP_HUM_SENSOR_0);
    currentHumidity = readHumidity(TEMP_HUM_SENSOR_0);
    currentLuminosity = readLuminosity(LUM_SENSOR_0, TSL2591_FULLSPECTRUM);

    // ------------------------------- Temperature -----------------------------
    // Too cold
    if (currentTemperature < desiredTemperature * 0.98) {
        controlRelay(HEATER, ON);
        controlRelay(FAN_IN, ON);
    }
    // Too hot
    else if (currentTemperature > desiredTemperature * 1.02) {
        controlRelay(HEATER, OFF);
        controlRelay(FAN_IN, ON);
        controlRelay(FAN_OUT, ON);
    }
    // Just right
    else {
        controlRelay(HEATER, OFF);
        controlRelay(FAN_IN, OFF);
    }

    // -------------------------------- Humidity -------------------------------
    // Too dry
    if (currentHumidity < desiredHumidity * 0.98) {
        controlRelay(HUMIDIFIER, ON);
        controlRelay(FAN_OUT, OFF);
    }// Too humid
    else if (currentHumidity > desiredHumidity * 1.02) {
        controlRelay(HUMIDIFIER, OFF);
        controlRelay(FAN_OUT, ON);
    }// Just right
    else {
        controlRelay(HUMIDIFIER, OFF);
        controlRelay(FAN_OUT, OFF);
    }

    // ------------------------------- Luminosity ------------------------------
    // No lights needed
    if (desiredLuminosity == 0) {
        controlRelay(LIGHTS, OFF);

        // Raise lights 1 cm at a time until lights are within 5 cm of the top of the enclosure
        while (readUltrasonicCm(PROXIMITY_SENSOR_LEFT) > 5 || readUltrasonicCm(PROXIMITY_SENSOR_RIGHT) > 5) {
            //            moveMotor(MOTOR_LEFT, UP, 1);
            //            moveMotor(MOTOR_RIGHT, UP, 1);
            CORETIMER_DelayMs(100);
        }
    }// Too dim
    else if (currentLuminosity < desiredLuminosity * 0.98) {
        controlRelay(LIGHTS, ON);

        // Lower lights 1 cm at a time until desired luminosity is met
        while (readLuminosity(LUM_SENSOR_0, TSL2591_FULLSPECTRUM) < desiredLuminosity) {
            //            moveMotor(MOTOR_LEFT, DOWN, 1);
            //            moveMotor(MOTOR_RIGHT, DOWN, 1);
        }
    }// Too bright
    else if (currentLuminosity > desiredLuminosity * 1.02) {
        controlRelay(LIGHTS, ON);

        // Raise lights 1 cm at a time until desired luminosity is met
        while (readLuminosity(LUM_SENSOR_0, TSL2591_FULLSPECTRUM) > desiredLuminosity) {
            //            moveMotor(MOTOR_LEFT, UP, 1);
            //            moveMotor(MOTOR_RIGHT, UP, 1);
        }
    }
}

// -----------------------------------------------------------------------------

void setup(void) {
    // Configure I2C slave interrupt
    // Interrupt is set to handle the following cases:
    //      - When I2C master (control panel) sends data to an address that
    //        matches I2C slave (this device)
    //      - Once I2C master is ready to send data to slave
    //      - Once I2C master is ready to receive data from slave
    //      - When I2C master is done with the transmission
    //
    // 'APP_I2C_SLAVE_Callback()' is executed upon the I2C slave interrupt
    I2C2_CallbackRegister(APP_I2C_SLAVE_Callback, 0);

    // Configure GPIO pins connected to the relays
    relaysInit();

    // Configure GPIO pins connected to the proximity sensors
    ultrasonicInit();

    // Configure GPIO pins for channel select on I2C mux
    I2CMuxInit();

    // Configure GPIO pins to enable motor driver and control direction
    motorInit();

    // Initialize 1602 LCD on I2C mux channel 0
    LCDInit(LCD_0);

    // Initialize temperature / humidity sensors on I2C mux channels 0 & 1
    Si7021Begin(TEMP_HUM_SENSOR_0);
    Si7021Begin(TEMP_HUM_SENSOR_1);

    // Initialize luminosity sensors on I2C mux channels 0 & 1
    TSL2591Begin(LUM_SENSOR_0, TSL2591_GAIN_MED, TSL2591_INTEGRATIONTIME_300MS);
    TSL2591Begin(LUM_SENSOR_1, TSL2591_GAIN_MED, TSL2591_INTEGRATIONTIME_300MS);
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void) {
    /* Initialize all modules */
    SYS_Initialize(NULL);

    // Initialize all external hardware peripherals 
    setup();

    // *************************************************************************
    // EXAMPLE: Read proximity sensors
    // *************************************************************************
    // uint8_t cmLeft = readUltrasonicCm( PROXIMITY_SENSOR_LEFT );
    // uint8_t cmRight = readUltrasonicCm( PROXIMITY_SENSOR_RIGHT );


    // *************************************************************************
    // EXAMPLE: Read luminosity sensors
    // *************************************************************************
     uint32_t luminosity0IR = readLuminosity(LUM_SENSOR_0, TSL2591_INFRARED);
    // uint32_t luminosity0Full = readLuminosity(LUM_SENSOR_0, TSL2591_FULLSPECTRUM);
    // uint32_t luminosity0Visible = readLuminosity(LUM_SENSOR_0, TSL2591_VISIBLE);
     uint32_t luminosity1IR = readLuminosity(LUM_SENSOR_1, TSL2591_INFRARED);
    // uint32_t luminosity1Full = readLuminosity(LUM_SENSOR_1, TSL2591_FULLSPECTRUM);
    // uint32_t luminosity1Visible = readLuminosity(LUM_SENSOR_1, TSL2591_VISIBLE);


    // *************************************************************************
    // EXAMPLE: Read temperature sensors
    // *************************************************************************
    // uint8_t temperature0 = readTemperature(TEMP_HUM_SENSOR_0);
    // uint8_t temperature1 = readTemperature(TEMP_HUM_SENSOR_1);


    // *************************************************************************
    // EXAMPLE: Read humidity sensors
    // *************************************************************************
    // uint8_t humidity0 = readHumidity(TEMP_HUM_SENSOR_0);
    // uint8_t humidity1 = readHumidity(TEMP_HUM_SENSOR_1);


    // *************************************************************************
    // EXAMPLE: Print on LCD
    // *************************************************************************
     LCDClear(LCD_0);
     LCDPrintString(LCD_0, "Lum0:");
     LCDPrint(LCD_0, luminosity0IR);
     LCDSetCursor(LCD_0, 0, 1);
     LCDPrintString(LCD_0, "Lum1:");
     LCDPrint(LCD_0, luminosity1IR);


    // *************************************************************************
    // EXAMPLE: Control motors
    // *************************************************************************
    // moveMotors( UP, 10000 );
    // moveMotors( DOWN, 10000 );
    // moveMotor( MOTOR_0, UP, 5000 );
    // moveMotor( MOTOR_0, DOWN, 5000 );
    // moveMotor( MOTOR_1, UP, 5000 );
    // moveMotor( MOTOR_1, DOWN, 5000 );


    // *************************************************************************
    // EXAMPLE: Control relays
    // *************************************************************************      
    // controlRelay( HEATER, ON );
    // controlRelay( HEATER, OFF );
    // controlRelay( FAN_IN, ON );
    // controlRelay( FAN_IN, OFF );
    // controlRelay( FAN_OUT, ON );
    // controlRelay( FAN_OUT, OFF );
    // controlRelay( HUMIDIFIER, ON );
    // controlRelay( HUMIDIFIER, OFF );
    // controlRelay( LIGHTS, ON );
    // controlRelay( LIGHTS, OFF );




    while (true) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();

        // climateControl();
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE);
}


/*******************************************************************************
 End of File
 */