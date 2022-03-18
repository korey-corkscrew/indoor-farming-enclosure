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

#define LCD_ADDRESS 0x27
// commands
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

    
void LCDInit( void );
void LCDClear( void );
void LCDHome( void );
void LCDSetCursor( uint8_t col, uint8_t row );
void LCDNoDisplay( void );
void LCDDisplay( void );
void LCDNoCursor();
void LCDCursor();
void LCDNoBlink();
void LCDBlink();
void LCDScrollDisplayLeft(void);
void LCDScrollDisplayRight(void);
void LCDLeftToRight(void);
void LCDRightToLeft(void);
void LCDAutoscroll(void);
void LCDNoAutoscroll(void);
void LCDNoBacklight(void);
void LCDBacklight(void);
bool LCDGetBacklight();
void LCDCommand(uint8_t value);
size_t LCDWrite(uint8_t value);
void LCDSend(uint8_t value, uint8_t mode);
void LCDWrite4Bits(uint8_t value);
void LCDExpanderWrite(uint8_t _data);
void LCDPulseEnable(uint8_t _data);
void LCDSetBacklight(uint8_t new_val);
void LCDPrint( int val );
void LCDPrintString( char *str );
char *itoa(int value);

#ifdef	__cplusplus
}
#endif

#endif	/* LIQUIDCRYSTALI2C_H */

