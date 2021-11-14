#ifndef LIGHTSENSORDRIVER_REFACTORED_H
#define LIGHTSENSORDRIVER_REFACTORED_H

#include "mbed.h"
#include <stdint.h>
#include <stdbool.h>
#include "I2C.h"

class LightSensors {
    // public methods
    public:
        // constructor
        LightSensors(I2C *i2c);
        
        // read the amount of Lux
        double readLux();

        // enter sleep mode
        void sleep();

        // wake up
        void wake();

    // pointer
    private:
        I2C *i2c;
    
    // adresses
    private:
        // Device Address
        const char I2C_ADDRESS = 0x29; // 7-bit Address
        // Register Address
        const char CONTROL_ADDR = 0x80;
        const char DATA_CH_1_0_ADDR = 0x88;
        const char DATA_CH_1_1_ADDR = 0x89;
        const char DATA_CH_0_0_ADDR= 0x8A;
        const char DATA_CH_0_1_ADDR = 0x8B;

    // private methods
    private:
        double readLux();
};

#endif