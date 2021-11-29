#include "LTR329ALS.h"
#include "mbed.h"
#include <stdint.h>
#include <stdbool.h>

LTR329ALS::LightSensors(I2C *i2c, uint8_t address)
{
    this->i2c = i2c;
    this->address = address;
}

void LTR329ALS::sleep()
{
    char cmd[2] {0};                        // empty data buffer
    cmd[0] = CONTROL_ADDR;                 // Pointer to CONTROL register
    cmd[1] = 0x00;                    // Data for CONTROL register (enter standby mode)
    i2c->write(address, cmd, 2);            // Send Address and command
}

void LTR329ALS::wake()
{
    char cmd[2] {0};                        // empty data buffer
    cmd[0] = CONTROL_ADDR;                 // Pointer to CONTROL register
    cmd[1] = 0x01;                    // Data for CONTROL register (enter active mode)
    i2c->write(address, cmd, 2);            // Send Address and command
}

double LTR329ALS::readLux()
{
    char data[4];
    char cmd[1] {0};                        // empty data buffer
    cmd[0] = DATA_CH_1_0_ADDR;              // Pointer to DATA register
    i2c->write(address, cmd, 1);
    i2c->read(address, cmd, 1);            // Read 1 byte from DATA register
    data[0] = cmd[0];

    cmd[0] = DATA_CH_1_1_ADDR;              // Pointer to DATA register
    i2c->write(address, cmd, 1);
    i2c->read(address, cmd, 1);            // Read 1 byte from DATA register
    data[1] = cmd[0];

    cmd[0] = DATA_CH_0_0_ADDR;              // Pointer to DATA register
    i2c->write(address, cmd, 1);
    i2c->read(address, cmd, 1);            // Read 1 byte from DATA register
    data[2] = cmd[0];

    cmd[0] = DATA_CH_0_1_ADDR;              // Pointer to DATA register
    i2c->write(address, cmd, 1);
    i2c->read(address, cmd, 1);            // Read 1 byte from DATA register
    data[3] = cmd[0];

    uint16_t CH1_ADC_Data = (data[1] << 8) | data[0]; // Combining lower and upper bytes to give 16-bit Ch1 data
    uint16_t CH0_ADC_Data = (data[3] << 8) | data[2]; // Combining lower and upper bytes to give 16-bit Ch0 data

    if(CH0_ADC_Data + CH1_ADC_Data == 0) return 0; // Avoid to Div by Zero 

    double ratio = CH1_ADC_Data / (CH0_ADC_Data + CH1_ADC_Data); 
    double lux = 0;

    if(ratio < 0.45) {
        return lux = (1.7743 * CH0_ADC_Data + 1.1059 * CH1_ADC_Data); 
    }
    else if(ratio < 0.64 &&  ratio >= 0.45) {
        return lux = (4.2785 * CH0_ADC_Data - 1.9548 * CH1_ADC_Data); 
    }
    else if(ratio < 0.85 &&  ratio >= 0.64) {
        return lux = (0.5926 * CH0_ADC_Data + 0.1185 * CH1_ADC_Data); 
    }
    else {
        return lux = 0; 
    }   
}