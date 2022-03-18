/* 
 * File:   LiquidCrystalI2C.h
 * Author: cbini
 *
 * Created on March 11, 2022, 6:16 PM
 */

#ifndef LIQUIDCRYSTALI2C_H
#define	LIQUIDCRYSTALI2C_H

#include "definitions.h"
#include "I2CMux.h"

#ifdef	__cplusplus
extern "C" {
#endif

// LCD I2C device address
#define LCD_ADDRESS 0x27

// I2C mux channel LCD is connected to.
// Be aware that the I2C mux only has 4 channels. Thus, only 4 of the same
// LCDs can be used. Currently, only one LCD is being used on channel 0. 
// Realistically, this is used to allow the user to easily switch which I2C mux 
// channel the LCD is connected to instead of supporting multiple LCDs.
#define LCD_0 CHANNEL_0
#define LCD_1 CHANNEL_1
#define LCD_2 CHANNEL_2
#define LCD_3 CHANNEL_3
    
// Commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0x04 // 00000100B  // Enable bit
#define Rw 0x02 // 00000010B  // Read/Write bit
#define Rs 0x01 // 00000001B  // Register select bit

    
void LCDInit( I2CMuxChannel channel );
void LCDClear( I2CMuxChannel channel );
void LCDHome( I2CMuxChannel channel );
void LCDSetCursor( I2CMuxChannel channel, uint8_t col, uint8_t row );
void LCDNoDisplay( I2CMuxChannel channel );
void LCDDisplay( I2CMuxChannel channel );
void LCDNoCursor(I2CMuxChannel channel);
void LCDCursor(I2CMuxChannel channel);
void LCDNoBlink(I2CMuxChannel channel);
void LCDBlink(I2CMuxChannel channel);
void LCDScrollDisplayLeft(I2CMuxChannel channel);
void LCDScrollDisplayRight(I2CMuxChannel channel);
void LCDLeftToRight(I2CMuxChannel channel);
void LCDRightToLeft(I2CMuxChannel channel);
void LCDAutoscroll(I2CMuxChannel channel);
void LCDNoAutoscroll(I2CMuxChannel channel);
void LCDNoBacklight(I2CMuxChannel channel);
void LCDBacklight(I2CMuxChannel channel);
void LCDCommand(I2CMuxChannel channel, uint8_t value);
size_t LCDWrite(I2CMuxChannel channel, uint8_t value);
void LCDSend(I2CMuxChannel channel, uint8_t value, uint8_t mode);
void LCDWrite4Bits(I2CMuxChannel channel, uint8_t value);
void LCDExpanderWrite(I2CMuxChannel channel, uint8_t _data);
void LCDPulseEnable(I2CMuxChannel channel, uint8_t _data);
void LCDSetBacklight(I2CMuxChannel channel, uint8_t new_val);
void LCDPrint( I2CMuxChannel channel, int val );
void LCDPrintString( I2CMuxChannel channel, char *str );
char *itoa(int value);

#ifdef	__cplusplus
}
#endif

#endif	/* LIQUIDCRYSTALI2C_H */

