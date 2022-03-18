/* 
 * File:   Si7021.h
 * Author: cbini
 *
 * Created on March 11, 2022, 6:38 PM
 */

#ifndef SI7021_H
#define	SI7021_H

#include "I2CMux.h"
#include "definitions.h"                // SYS function prototypes

#ifdef	__cplusplus
extern "C" {
#endif

// Device I2C slave address
#define SI7021_DEFAULT_ADDRESS 0x40

// I2C mux channels that connect the sensors.
// Be aware that the I2C mux only has 4 channels. Thus, only 4 of the same
// sensors can be used. Currently, only two sensors are being used on channels
// 0 and 1.
#define TEMP_HUM_SENSOR_0 CHANNEL_0
#define TEMP_HUM_SENSOR_1 CHANNEL_1
#define TEMP_HUM_SENSOR_2 CHANNEL_2
#define TEMP_HUM_SENSOR_3 CHANNEL_3

    
// Commands
#define SI7021_MEASRH_HOLD_CMD                                                 \
  0xE5 /**< Measure Relative Humidity, Hold Master Mode */
#define SI7021_MEASRH_NOHOLD_CMD                                               \
  0xF5 /**< Measure Relative Humidity, No Hold Master Mode */
#define SI7021_MEASTEMP_HOLD_CMD                                               \
  0xE3 /**< Measure Temperature, Hold Master Mode */
#define SI7021_MEASTEMP_NOHOLD_CMD                                             \
  0xF3 /**< Measure Temperature, No Hold Master Mode */
#define SI7021_READPREVTEMP_CMD                                                \
  0xE0 /**< Read Temperature Value from Previous RH Measurement */
#define SI7021_RESET_CMD 0xFE           /**< Reset Command */
#define SI7021_WRITERHT_REG_CMD 0xE6    /**< Write RH/T User Register 1 */
#define SI7021_READRHT_REG_CMD 0xE7     /**< Read RH/T User Register 1 */
#define SI7021_WRITEHEATER_REG_CMD 0x51 /**< Write Heater Control Register */
#define SI7021_READHEATER_REG_CMD 0x11  /**< Read Heater Control Register */
#define SI7021_REG_HTRE_BIT 0x02        /**< Control Register Heater Bit */
#define SI7021_ID1_CMD 0xFA0F           /**< Read Electronic ID 1st Byte */
#define SI7021_ID2_CMD 0xFCC9           /**< Read Electronic ID 2nd Byte */
#define SI7021_FIRMVERS_CMD 0x84B8      /**< Read Firmware Revision */
#define SI7021_REV_1 0xff /**< Sensor revision 1 */
#define SI7021_REV_2 0x20 /**< Sensor revision 2 */
    

// Functions
void Si7021Reset(I2CMuxChannel channel);
bool Si7021Begin(I2CMuxChannel channel);
uint8_t readTemperature(I2CMuxChannel channel);
uint8_t readHumidity(I2CMuxChannel channel);

#ifdef	__cplusplus
}
#endif

#endif	/* SI7021_H */

