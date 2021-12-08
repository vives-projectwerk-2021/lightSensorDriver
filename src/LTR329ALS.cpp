#include "LTR329ALS.h"
#include "mbed.h"
#include <stdint.h>
#include <stdbool.h>

LTR329ALS::LTR329ALS(I2C *i2c, uint8_t address)
{
    this->i2c = i2c;
    this->address = address;
}

/** Set the register to enter sleep mode
 * @return 0 on success
 */
bool LTR329ALS::sleep()
{
    char cmd[2] {0};                        // empty data buffer
    cmd[0] = CONTROL_ADDR;                  // Pointer to CONTROL register
    cmd[1] = 0x00;                          // Data for CONTROL register (enter standby mode)
    return i2c->write(address, cmd, 2);     // Send Address and command
}

/** Set the register to leave sleep mode
 * @return 0 on success
 */
bool LTR329ALS::wake()
{
    char cmd[2] {0};                        // empty data buffer
    cmd[0] = CONTROL_ADDR;                  // Pointer to CONTROL register
    cmd[1] = 0x01;                          // Data for CONTROL register (enter active mode)
    return i2c->write(address, cmd, 2);     // Send Address and command
}

double LTR329ALS::readLux(bool* error)
{
    bool ack;
    char data[4] {0};
    char cmd[1] {0};                        // empty data buffer
    cmd[0] = DATA_CH_1_0_ADDR;              // Pointer to DATA register
    ack = i2c->write(address, cmd, 1);
    if(ack) {
        if(error != nullptr) *error = true;
        return 0;
    }
    ack = i2c->read(address, cmd, 1);            // Read 1 byte from DATA register
    if(ack) {
        if(error != nullptr) *error = true;
        return 0;
    }
    data[0] = cmd[0];

    cmd[0] = DATA_CH_1_1_ADDR;              // Pointer to DATA register
    ack = i2c->write(address, cmd, 1);
    if(ack) {
        if(error != nullptr) *error = true;
        return 0;
    }
    ack = i2c->read(address, cmd, 1);            // Read 1 byte from DATA register
    if(ack) {
        if(error != nullptr) *error = true;
        return 0;
    }
    data[1] = cmd[0];

    cmd[0] = DATA_CH_0_0_ADDR;              // Pointer to DATA register
    ack = i2c->write(address, cmd, 1);
    if(ack) {
        if(error != nullptr) *error = true;
        return 0;
    }
    ack = i2c->read(address, cmd, 1);            // Read 1 byte from DATA register
    if(ack) {
        if(error != nullptr) *error = true;
        return 0;
    }
    data[2] = cmd[0];

    cmd[0] = DATA_CH_0_1_ADDR;              // Pointer to DATA register
    ack = i2c->write(address, cmd, 1);
    if(ack) {
        if(error != nullptr) *error = true;
        return 0;
    }
    ack = i2c->read(address, cmd, 1);            // Read 1 byte from DATA register
    if(ack) {
        if(error != nullptr) *error = true;
        return 0;
    }
    data[3] = cmd[0];

    if(error != nullptr) *error = false;

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