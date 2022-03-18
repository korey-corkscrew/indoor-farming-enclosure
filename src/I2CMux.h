/* 
 * File:   enclosure.h
 * Author: cbini
 *
 * Created on March 11, 2022, 6:50 PM
 */

#ifndef ENCLOSURE_H
#define	ENCLOSURE_H

#include "definitions.h"                // SYS function prototypes

#ifdef	__cplusplus
extern "C" {
#endif

// I2C mux channel select
#define I2C_MUX_A0              GPIO_PIN_RE1
#define I2C_MUX_A1              GPIO_PIN_RE0

#define I2C_MUX_ADDRESS         0x70

// I2C master mux channels
typedef enum {
    CHANNEL_0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3
} I2CMuxChannel;

extern I2CMuxChannel currentChannel;

void I2C1Write(uint16_t address, uint8_t* wdata, size_t wlength);
void I2C1Read(uint16_t address, uint8_t* rdata, size_t rlength);
void I2C1WriteRead(uint16_t address, uint8_t* wdata, size_t wlength, uint8_t* rdata, size_t rlength);
void setI2CMuxChannel(I2CMuxChannel channel);
void I2CMuxInit(void);


#ifdef	__cplusplus
}
#endif

#endif	/* ENCLOSURE_H */

