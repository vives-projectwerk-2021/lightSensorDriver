#ifndef LTR329ALS_H
#define LTR329ALS_H

#include "mbed.h"
#include <stdint.h>
#include <stdbool.h>
#include "I2C.h"

class LTR329ALS {
    // public methods
    public:
        // constructor
        LTR329ALS(I2C *i2c, uint8_t address);
        
        // read the amount of Lux
        double readLux();

        // enter sleep mode
        void sleep();

        // wake up
        void wake();

    // pointer
    private:
        I2C *i2c;
        uint8_t address;
    
    // adresses
    private:
        // Register Address
        const char CONTROL_ADDR = 0x80;
        const char DATA_CH_1_0_ADDR = 0x88;
        const char DATA_CH_1_1_ADDR = 0x89;
        const char DATA_CH_0_0_ADDR= 0x8A;
        const char DATA_CH_0_1_ADDR = 0x8B;
};

#endif