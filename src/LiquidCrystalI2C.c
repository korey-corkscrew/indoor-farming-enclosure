#include "LiquidCrystalI2C.h"


uint8_t _addr;
uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _cols;
uint8_t _rows;
uint8_t _charsize;
uint8_t _backlightval;


void LCDInit( void ) {
    _cols = 16;
    _rows = 2;
    _backlightval = LCD_BACKLIGHT;
    
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    _displayfunction |= LCD_2LINE;
    CORETIMER_DelayMs( 50 );
    
    LCDExpanderWrite( _backlightval );
    CORETIMER_DelayMs( 1000 );
    
    LCDWrite4Bits( 0x03 << 4 );
    CORETIMER_DelayUs( 4500 );
    
    LCDWrite4Bits( 0x03 << 4 );
    CORETIMER_DelayUs( 4500 );
    
    LCDWrite4Bits( 0x03 << 4 );
    CORETIMER_DelayUs( 150 );
    
    LCDWrite4Bits( 0x02 << 4 );
    
    LCDCommand( LCD_FUNCTIONSET | _displayfunction );
    
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	
    LCDDisplay();

	// clear it off
	LCDClear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	LCDCommand(LCD_ENTRYMODESET | _displaymode);

	LCDHome();
    LCDBacklight();
    LCDClear();
    LCDBlink();
}

void LCDClear( void ) {
    LCDCommand( LCD_CLEARDISPLAY );
    CORETIMER_DelayUs( 2000 );
}

void LCDHome( void ) {
    LCDCommand( LCD_RETURNHOME );
    CORETIMER_DelayUs( 2000 );
}

void LCDSetCursor( uint8_t col, uint8_t row ) {
    uint8_t row_offsets[2] = { 0x00, 0x40 };
    if( row > 1 ) {
        row = 1;
    }
    LCDCommand( LCD_SETDDRAMADDR | (col + row_offsets[row]) );
}

// Turn the display on/off (quickly)
void LCDNoDisplay( void ) {
	_displaycontrol &= ~LCD_DISPLAYON;
	LCDCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCDDisplay( void ) {
	_displaycontrol |= LCD_DISPLAYON;
	LCDCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LCDNoCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	LCDCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCDCursor() {
	_displaycontrol |= LCD_CURSORON;
	LCDCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LCDNoBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	LCDCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LCDBlink() {
	_displaycontrol |= LCD_BLINKON;
	LCDCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LCDScrollDisplayLeft(void) {
	LCDCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LCDScrollDisplayRight(void) {
	LCDCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LCDLeftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	LCDCommand(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LCDRightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	LCDCommand(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LCDAutoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	LCDCommand(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LCDNoAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	LCDCommand(LCD_ENTRYMODESET | _displaymode);
}

// Turn the (optional) backlight off/on
void LCDNoBacklight(void) {
	_backlightval = LCD_NOBACKLIGHT;
	LCDExpanderWrite(0);
}

void LCDBacklight(void) {
	_backlightval = LCD_BACKLIGHT;
	LCDExpanderWrite(0);
}

bool LCDGetBacklight() {
  return _backlightval == LCD_BACKLIGHT;
}


/*********** mid level commands, for sending data/cmds */

inline void LCDCommand(uint8_t value) {
	LCDSend(value, 0);
}

inline size_t LCDWrite(uint8_t value) {
	LCDSend(value, Rs);
	return 1;
}


///************ low level data pushing commands **********/

// write either command or data
void LCDSend(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	LCDWrite4Bits((highnib)|mode);
	LCDWrite4Bits((lownib)|mode);
}

void LCDWrite4Bits(uint8_t value) {
	LCDExpanderWrite(value);
	LCDPulseEnable(value);
}

void LCDExpanderWrite(uint8_t _data){
    setI2CMuxChannel( CHANNEL_0 );
    uint8_t buffer[1] = { _data | _backlightval };
    I2C1Write( LCD_ADDRESS, buffer, 1 );
}

void LCDPulseEnable(uint8_t _data){
	LCDExpanderWrite(_data | En);	// En high
	CORETIMER_DelayUs(1);		// enable pulse must be >450ns

	LCDExpanderWrite(_data & ~En);	// En low
	CORETIMER_DelayUs(50);		// commands need > 37us to settle
}

void LCDSetBacklight(uint8_t new_val){
	if (new_val) {
		LCDBacklight();		// turn backlight on
	} else {
		LCDNoBacklight();		// turn backlight off
	}
}

void LCDPrint( int val ) {
    char *string = itoa(val);
    for(int i = 0; i < sizeof(string); i++) {
        if(string[i] < 0x30 || string[i] > 0x39){
            return;
        }
        LCDWrite(string[i]);
    }
}

void LCDPrintString( char *str ) {
    for(int i = 0; i < sizeof(str); i++) {
        if(str[i] > 0x20 || str[i] < 0x7E) {
            LCDWrite(str[i]);
        }
    }
}

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