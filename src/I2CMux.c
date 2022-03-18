#include "I2CMux.h"

I2CMuxChannel currentChannel = CHANNEL_0;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void I2C1Write(uint16_t address, uint8_t* wdata, size_t wlength) {
    // Wait for I2C master bus to clear
    while (I2C1_IsBusy());
    
    // Write I2C data from master to slave
    I2C1_Write(address, wdata, wlength);
    
    // Wait for I2C master bus to clear
    while (I2C1_IsBusy());
}

// -----------------------------------------------------------------------------

void I2C1Read(uint16_t address, uint8_t* rdata, size_t rlength) {
    while (I2C1_IsBusy());
    I2C1_Read(address, rdata, rlength);
    while (I2C1_IsBusy());
}

// -----------------------------------------------------------------------------

void I2C1WriteRead(uint16_t address, uint8_t* wdata, size_t wlength, uint8_t* rdata, size_t rlength) {
    while (I2C1_IsBusy());
    I2C1_WriteRead(address, wdata, wlength, rdata, rlength);
    while (I2C1_IsBusy());
}

// -----------------------------------------------------------------------------

/**
 * @def Configures the I2C mux to the desired channel by sending a corresponding
 *      byte to the mux.
 * @param channel:
 *      Desired I2C mux channel.
 */
void setI2CMuxChannel(I2CMuxChannel channel) {
    // Only change if new I2C mux channel
    if( channel != currentChannel ) {
        uint8_t buffer[1];

        // Determine which byte to send to the I2C mux
        switch (channel) {
            case CHANNEL_0:
                buffer[0] = 0x01;
                break;
            case CHANNEL_1:
                buffer[0] = 0x02;
                break;
            case CHANNEL_2:
                buffer[0] = 0x03;
                break;
            case CHANNEL_3:
                buffer[0] = 0x04;
                break;
            default:
                return;
        }
        
        currentChannel = channel;
    
        // Send the desired channel byte to the I2C mux
        I2C1Write(I2C_MUX_ADDRESS, buffer, 1);
    }
}

// -----------------------------------------------------------------------------

/**
 * @def Set the GPIO pins connected to the I2C mux to change the device address.
 *      I2C mux device address is 0x70 by default.
 *      A1      A0      Mux Address
 *      0       0       0x70
 *      0       1       0x71
 *      1       0       0x72
 *      1       1       0x73
 *      
 */
void I2CMuxInit(void) {
    // Configure pins as outputs
    GPIO_PinOutputEnable(I2C_MUX_A0);
    GPIO_PinOutputEnable(I2C_MUX_A1);
    
    // Set device address to 0x70
    GPIO_PinClear(I2C_MUX_A0);
    GPIO_PinClear(I2C_MUX_A1);
}