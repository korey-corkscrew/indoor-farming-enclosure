#include "Si7021.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 * @def
 * @param sensor
 */
void Si7021Reset(I2CMuxChannel channel) {
    uint8_t buffer[1] = {SI7021_RESET_CMD};
    I2C1Write(channel, SI7021_DEFAULT_ADDRESS, buffer, 1);
    CORETIMER_DelayMs(50);
}

// -----------------------------------------------------------------------------

/**
 * @def
 * @param sensor
 * @return 
 */
bool Si7021Begin(I2CMuxChannel channel) {
    Si7021Reset(channel);
    uint8_t buffer[1] = {SI7021_READRHT_REG_CMD};
    I2C1WriteRead(channel, SI7021_DEFAULT_ADDRESS, buffer, 1, buffer, 1);
    if (buffer[0] != 0x3A) {
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------

/**
 * @def
 * @param sensor
 * @return 
 */
uint8_t readTemperature(I2CMuxChannel channel) {
    // Begin temperature reading
    uint8_t buffer[3] = {SI7021_MEASTEMP_NOHOLD_CMD, 0, 0};
    I2C1Write(channel, SI7021_DEFAULT_ADDRESS, buffer, 1);

    // Read current temperature from sensor
    I2C1Read(channel, SI7021_DEFAULT_ADDRESS, buffer, 3);
    uint16_t temp = buffer[0] << 8 | buffer[1];

    // Convert to ^F
    float temperature = temp;
    temperature *= 175.72;
    temperature /= 65536;
    temperature -= 46.85;
    temperature *= 9;
    temperature /= 5;
    temperature += 32;
    return (uint8_t) temperature;
}

// -----------------------------------------------------------------------------

/**
 * @def
 * @param sensor
 * @return 
 */
uint8_t readHumidity(I2CMuxChannel channel) {
    // Begin humidity reading
    uint8_t buffer[3] = {SI7021_MEASRH_NOHOLD_CMD, 0, 0};
    I2C1Write(channel, SI7021_DEFAULT_ADDRESS, buffer, 1);

    // Read current humidity from sensor
    I2C1Read(channel, SI7021_DEFAULT_ADDRESS, buffer, 3);
    uint16_t hum = buffer[0] << 8 | buffer[1];

    float humidity = hum;
    humidity *= 125;
    humidity /= 65536;
    humidity -= 6;

    return (uint8_t) humidity > 100.0 ? 100.0 : humidity;
}
