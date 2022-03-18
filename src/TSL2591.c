#include "TSL2591.h"

tsl2591IntegrationTime_t _integration;
tsl2591Gain_t _gain;

// -----------------------------------------------------------------------------

void TSL2591Enable(I2CMuxChannel channel) {
    uint8_t buffer[2] = {
        TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE,
        TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN | TSL2591_ENABLE_NPIEN
    };

    I2C1Write(channel, TSL2591_ADDR, buffer, 2);
}

// -----------------------------------------------------------------------------

void TSL2591Disable(I2CMuxChannel channel) {
    uint8_t buffer[2] = {TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE, TSL2591_ENABLE_POWEROFF};

    I2C1Write(channel, TSL2591_ADDR, buffer, 2);
}

// -----------------------------------------------------------------------------

bool TSL2591Begin(I2CMuxChannel channel, tsl2591IntegrationTime_t integration, tsl2591Gain_t gain) {
    // Read device ID
    uint8_t buffer[1] = {TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_ID};
    I2C1WriteRead(channel, TSL2591_ADDR, buffer, 1, buffer, 1);

    // Confirm the device is connected
    if (buffer[0] != 0x50) {
        return false;
    }
    TSL2591SetGainTiming(channel, gain, integration);
    return true;
}

// -----------------------------------------------------------------------------

void TSL2591SetGainTiming(I2CMuxChannel channel, tsl2591Gain_t gain, tsl2591IntegrationTime_t integration) {
    TSL2591Enable(channel);
    _gain = gain;
    _integration = integration;
    uint8_t buffer[2] = {TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL, _integration | _gain};
    I2C1Write(channel, TSL2591_ADDR, buffer, 2);
    TSL2591Disable(channel);
}

// -----------------------------------------------------------------------------

uint32_t TSL2591GetFullLuminosity(I2CMuxChannel channel) {
    // Enable the device
    TSL2591Enable(channel);

    // Wait x ms for ADC to complete
    for (uint8_t d = 0; d <= _integration; d++) {
        CORETIMER_DelayMs(120);
    }

    // CHAN0 must be read before CHAN1
    uint32_t x;
    uint16_t y;
    uint8_t buffer[2] = {TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW, 0};
    I2C1WriteRead(channel, TSL2591_ADDR, buffer, 1, buffer, 2);
    y = (uint16_t) buffer[1] << 8 | (uint16_t) buffer[0];

    buffer[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW;
    I2C1WriteRead(channel, TSL2591_ADDR, buffer, 1, buffer, 2);
    x = (uint16_t) buffer[1] << 8 | (uint16_t) buffer[0];

    x <<= 16;
    x |= y;

    TSL2591Disable(channel);

    return x;
}

// -----------------------------------------------------------------------------

uint16_t readLuminosity(I2CMuxChannel channel, Channel lumChannel) {
    uint32_t x = TSL2591GetFullLuminosity(channel);

    switch (lumChannel) {
        case TSL2591_FULLSPECTRUM:
            // Reads two byte value from channel 0 (visible + infrared)
            return (x & 0xFFFF);
        case TSL2591_INFRARED:
            // Reads two byte value from channel 1 (infrared)
            return (x >> 16);
        case TSL2591_VISIBLE:
            // Reads all and subtracts out just the visible
            return ((x & 0xFFFF) - (x >> 16));
        default:
            return 0;
    }
}

float TSL2591CalculateLux(uint16_t ch0, uint16_t ch1) {
  float atime, again;
  float cpl, lux;// lux1, lux2, lux;
//  uint32_t chan0, chan1;

  // Check for overflow conditions first
  if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF)) {
    // Signal an overflow
    return -1;
  }

  // Note: This algorithm is based on preliminary coefficients
  // provided by AMS and may need to be updated in the future

  switch (_integration) {
  case TSL2591_INTEGRATIONTIME_100MS:
    atime = 100.0F;
    break;
  case TSL2591_INTEGRATIONTIME_200MS:
    atime = 200.0F;
    break;
  case TSL2591_INTEGRATIONTIME_300MS:
    atime = 300.0F;
    break;
  case TSL2591_INTEGRATIONTIME_400MS:
    atime = 400.0F;
    break;
  case TSL2591_INTEGRATIONTIME_500MS:
    atime = 500.0F;
    break;
  case TSL2591_INTEGRATIONTIME_600MS:
    atime = 600.0F;
    break;
  default: // 100ms
    atime = 100.0F;
    break;
  }

  switch (_gain) {
  case TSL2591_GAIN_LOW:
    again = 1.0F;
    break;
  case TSL2591_GAIN_MED:
    again = 25.0F;
    break;
  case TSL2591_GAIN_HIGH:
    again = 428.0F;
    break;
  case TSL2591_GAIN_MAX:
    again = 9876.0F;
    break;
  default:
    again = 1.0F;
    break;
  }

  // cpl = (ATIME * AGAIN) / DF
  cpl = (atime * again) / TSL2591_LUX_DF;

  // Original lux calculation (for reference sake)
  // lux1 = ( (float)ch0 - (TSL2591_LUX_COEFB * (float)ch1) ) / cpl;
  // lux2 = ( ( TSL2591_LUX_COEFC * (float)ch0 ) - ( TSL2591_LUX_COEFD *
  // (float)ch1 ) ) / cpl; lux = lux1 > lux2 ? lux1 : lux2;

  // Alternate lux calculation 1
  // See: https://github.com/adafruit/Adafruit_TSL2591_Library/issues/14
  lux = (((float)ch0 - (float)ch1)) * (1.0F - ((float)ch1 / (float)ch0)) / cpl;

  // Alternate lux calculation 2
  // lux = ( (float)ch0 - ( 1.7F * (float)ch1 ) ) / cpl;

  // Signal I2C had no errors
  return lux;
}

float readLux(I2CMuxChannel channel) {
    uint32_t lum = TSL2591GetFullLuminosity( channel );
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    
    return TSL2591CalculateLux(full, ir);
}