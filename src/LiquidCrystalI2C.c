#include "LiquidCrystalI2C.h"

uint8_t _addr;
uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _cols;
uint8_t _rows;
uint8_t _charsize;
uint8_t _backlightval;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 * @def
 */
void LCDInit( I2CMuxChannel channel ) {
    _cols = 16;
    _rows = 2;
    _backlightval = LCD_BACKLIGHT;
    
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    _displayfunction |= LCD_2LINE;
    CORETIMER_DelayMs( 50 );
    
    LCDExpanderWrite( channel, _backlightval );
    CORETIMER_DelayMs( 1000 );
    
    LCDWrite4Bits( channel, 0x03 << 4 );
    CORETIMER_DelayUs( 4500 );
    
    LCDWrite4Bits( channel, 0x03 << 4 );
    CORETIMER_DelayUs( 4500 );
    
    LCDWrite4Bits( channel, 0x03 << 4 );
    CORETIMER_DelayUs( 150 );
    
    LCDWrite4Bits( channel, 0x02 << 4 );
    
    LCDCommand( channel, LCD_FUNCTIONSET | _displayfunction );
    
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	
    LCDDisplay(channel);

	// clear it off
	LCDClear(channel);

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	LCDCommand(channel, LCD_ENTRYMODESET | _displaymode);

    // Move cursor to 0, 0
	LCDHome(channel);
    
    // Turn backlight on
    LCDBacklight(channel);
    
    // Turn cursor blink on
    LCDBlink(channel);
}

// -----------------------------------------------------------------------------

/**
 * @def
 */
void LCDClear( I2CMuxChannel channel ) {
    LCDCommand( channel, LCD_CLEARDISPLAY );
    CORETIMER_DelayUs( 2000 );
}

// -----------------------------------------------------------------------------

/**
 * @def
 */
void LCDHome( I2CMuxChannel channel ) {
    LCDCommand( channel, LCD_RETURNHOME );
    CORETIMER_DelayUs( 2000 );
}

// -----------------------------------------------------------------------------

/**
 * @def
 * @param col
 * @param row
 */
void LCDSetCursor( I2CMuxChannel channel, uint8_t col, uint8_t row ) {
    uint8_t row_offsets[2] = { 0x00, 0x40 };
    if( row > 1 ) {
        row = 1;
    }
    LCDCommand( channel, LCD_SETDDRAMADDR | (col + row_offsets[row]) );
}

// -----------------------------------------------------------------------------

/**
 * @def Turn the display off
 */
void LCDNoDisplay( I2CMuxChannel channel ) {
	_displaycontrol &= ~LCD_DISPLAYON;
	LCDCommand(channel, LCD_DISPLAYCONTROL | _displaycontrol);
}

// -----------------------------------------------------------------------------

/**
 * @def Turn the display on
 */
void LCDDisplay( I2CMuxChannel channel ) {
	_displaycontrol |= LCD_DISPLAYON;
	LCDCommand(channel, LCD_DISPLAYCONTROL | _displaycontrol);
}

// -----------------------------------------------------------------------------

/**
 * @def Turns the underline cursor off
 */
void LCDNoCursor( I2CMuxChannel channel ) {
	_displaycontrol &= ~LCD_CURSORON;
	LCDCommand(channel, LCD_DISPLAYCONTROL | _displaycontrol);
}

// -----------------------------------------------------------------------------

/**
 * @def Turns the underline cursor on
 */
void LCDCursor( I2CMuxChannel channel ) {
	_displaycontrol |= LCD_CURSORON;
	LCDCommand(channel, LCD_DISPLAYCONTROL | _displaycontrol);
}

// -----------------------------------------------------------------------------

/**
 * @def Turn off the blinking cursor
 */
void LCDNoBlink( I2CMuxChannel channel ) {
	_displaycontrol &= ~LCD_BLINKON;
	LCDCommand(channel, LCD_DISPLAYCONTROL | _displaycontrol);
}

// -----------------------------------------------------------------------------

/**
 * @def Turn on the blinking cursor
 */
void LCDBlink( I2CMuxChannel channel ) {
	_displaycontrol |= LCD_BLINKON;
	LCDCommand(channel, LCD_DISPLAYCONTROL | _displaycontrol);
}

// -----------------------------------------------------------------------------

// These commands scroll the display without changing the RAM
void LCDScrollDisplayLeft(I2CMuxChannel channel) {
	LCDCommand(channel, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

// -----------------------------------------------------------------------------

void LCDScrollDisplayRight(I2CMuxChannel channel) {
	LCDCommand(channel, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// -----------------------------------------------------------------------------

// This is for text that flows Left to Right
void LCDLeftToRight(I2CMuxChannel channel) {
	_displaymode |= LCD_ENTRYLEFT;
	LCDCommand(channel, LCD_ENTRYMODESET | _displaymode);
}

// -----------------------------------------------------------------------------

// This is for text that flows Right to Left
void LCDRightToLeft(I2CMuxChannel channel) {
	_displaymode &= ~LCD_ENTRYLEFT;
	LCDCommand(channel, LCD_ENTRYMODESET | _displaymode);
}

// -----------------------------------------------------------------------------

// This will 'right justify' text from the cursor
void LCDAutoscroll(I2CMuxChannel channel) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	LCDCommand(channel, LCD_ENTRYMODESET | _displaymode);
}

// -----------------------------------------------------------------------------

// This will 'left justify' text from the cursor
void LCDNoAutoscroll(I2CMuxChannel channel) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	LCDCommand(channel, LCD_ENTRYMODESET | _displaymode);
}

// -----------------------------------------------------------------------------

// Turn the (optional) backlight off/on
void LCDNoBacklight(I2CMuxChannel channel) {
	_backlightval = LCD_NOBACKLIGHT;
	LCDExpanderWrite(channel, 0);
}

// -----------------------------------------------------------------------------

void LCDBacklight(I2CMuxChannel channel) {
	_backlightval = LCD_BACKLIGHT;
	LCDExpanderWrite(channel, 0);
}

// -----------------------------------------------------------------------------

inline void LCDCommand(I2CMuxChannel channel, uint8_t value) {
	LCDSend(channel, value, 0);
}

// -----------------------------------------------------------------------------

inline size_t LCDWrite(I2CMuxChannel channel, uint8_t value) {
	LCDSend(channel, value, Rs);
	return 1;
}

// -----------------------------------------------------------------------------

// write either command or data
void LCDSend(I2CMuxChannel channel, uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	LCDWrite4Bits(channel, (highnib)|mode);
	LCDWrite4Bits(channel, (lownib)|mode);
}

// -----------------------------------------------------------------------------

void LCDWrite4Bits(I2CMuxChannel channel, uint8_t value) {
	LCDExpanderWrite(channel, value);
	LCDPulseEnable(channel, value);
}

// -----------------------------------------------------------------------------

void LCDExpanderWrite(I2CMuxChannel channel, uint8_t _data){
    uint8_t buffer[1] = { _data | _backlightval };
    I2C1Write( channel, LCD_ADDRESS, buffer, 1 );
}

// -----------------------------------------------------------------------------

void LCDPulseEnable(I2CMuxChannel channel, uint8_t _data){
	LCDExpanderWrite(channel, _data | En);	// En high
	CORETIMER_DelayUs(1);		// enable pulse must be >450ns

	LCDExpanderWrite(channel, _data & ~En);	// En low
	CORETIMER_DelayUs(50);		// commands need > 37us to settle
}

// -----------------------------------------------------------------------------

void LCDPrint( I2CMuxChannel channel, int val ) {
    char *string = itoa(val);
    for(int i = 0; i < sizeof(string); i++) {
        if(string[i] < 0x30 || string[i] > 0x39){
            return;
        }
        LCDWrite(channel, string[i]);
    }
}

// -----------------------------------------------------------------------------

void LCDPrintString( I2CMuxChannel channel, char *str ) {
    for(int i = 0; i < sizeof(str); i++) {
        if(str[i] > 0x20 || str[i] < 0x7E) {
            LCDWrite(channel, str[i]);
        }
    }
}

// -----------------------------------------------------------------------------

/**
 * @def Converts an int to an array of characters
 * @param value:
 *      Int to convert
 * @return Array of characters
 */
char *itoa(int value) 
 {
     static char buffer[12];        // 12 bytes is big enough for an INT32
     int original = value;        // save original value
 
     int c = sizeof(buffer)-1;
 
     buffer[c] = '\n';                // write trailing null in last byte of buffer    
 
     if (value < 0)                 // if it's negative, note that and take the absolute value
         value = -value;
     
     do                             // write least significant digit of value that's left
     {
         buffer[--c] = (value % 10) + '0';    
         value /= 10;
     } while (value);
 
     if (original < 0) 
         buffer[--c] = '-';
 
     return &buffer[c];
 }