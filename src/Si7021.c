#include "Si7021.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void Si7021Reset(TempertaureHumiditySensor sensor) {
    switch (sensor) {
        case TEMP_HUM_SENSOR_0:
            setI2CMuxChannel(CHANNEL_0);
            break;
        case TEMP_HUM_SENSOR_1:
            setI2CMuxChannel(CHANNEL_1);
            break;
    }
    uint8_t buffer[1] = {SI7021_RESET_CMD};
    I2C1Write(SI7021_DEFAULT_ADDRESS, buffer, 1);
    CORETIMER_DelayMs(50);
}

// -----------------------------------------------------------------------------

bool Si7021Begin(TempertaureHumiditySensor sensor) {
    switch (sensor) {
        case TEMP_HUM_SENSOR_0:
            setI2CMuxChannel(CHANNEL_0);
            break;
        case TEMP_HUM_SENSOR_1:
            setI2CMuxChannel(CHANNEL_1);
            break;
    }
    Si7021Reset(sensor);
    uint8_t buffer[1] = {SI7021_READRHT_REG_CMD};
    I2C1WriteRead(SI7021_DEFAULT_ADDRESS, buffer, 1, buffer, 1);
    if (buffer[0] != 0x3A) {
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------

uint8_t readTemperature(TempertaureHumiditySensor sensor) {
    // Select I2C mux channel
    switch (sensor) {
        case TEMP_HUM_SENSOR_0:
            // Set I2C mux to channel 0
            setI2CMuxChannel(CHANNEL_0);
            break;
        case TEMP_HUM_SENSOR_1:
            // Set I2C mux to channel 1
            setI2CMuxChannel(CHANNEL_1);
            break;
    }

    // Begin temperature reading
    uint8_t buffer[3] = {SI7021_MEASTEMP_NOHOLD_CMD, 0, 0};
    I2C1Write(SI7021_DEFAULT_ADDRESS, buffer, 1);

    // Read current temperature from sensor
    I2C1Read(SI7021_DEFAULT_ADDRESS, buffer, 3);
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

uint8_t readHumidity(TempertaureHumiditySensor sensor) {
    // Select I2C mux channel
    switch (sensor) {
        case TEMP_HUM_SENSOR_0:
            // Set I2C mux to channel 0
            setI2CMuxChannel(CHANNEL_0);
            break;
        case TEMP_HUM_SENSOR_1:
            // Set I2C mux to channel 1
            setI2CMuxChannel(CHANNEL_1);
            break;
    }

    // Begin humidity reading
    uint8_t buffer[3] = {SI7021_MEASRH_NOHOLD_CMD, 0, 0};
    I2C1Write(SI7021_DEFAULT_ADDRESS, buffer, 1);

    // Read current humidity from sensor
    I2C1Read(SI7021_DEFAULT_ADDRESS, buffer, 3);
    uint16_t hum = buffer[0] << 8 | buffer[1];

    float humidity = hum;
    humidity *= 125;
    humidity /= 65536;
    humidity -= 6;

    return (uint8_t) humidity > 100.0 ? 100.0 : humidity;
}
