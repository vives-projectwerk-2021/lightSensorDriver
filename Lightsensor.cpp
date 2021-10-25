#include "LTR329ALS01.h"
#include "mbed.h"
#include <stdint.h>
#include <stdbool.h>

I2C i2c(I2C_SDA, I2C_SCL);   

int main ()
{
    float lux; 
    float pFactor = LTR329ALS01_PFACTOR;
    uint16_t ALS_CH0_ADC_Data, ALS_CH1_ADC_Data;
    char Data[3];

    wait(100); // Initial startuptime

    // Enable ALS
    i2c.write(LTR329ALS01_I2C_ADDRESS, LTR329ALS01_ADDR_ALS_CONTROL, LTR329ALS01_ALS_GAIN_x1);

    // The ALS_MEAS_RATE register controls the ALS integration time and measurement rate.
    // Default setting of the register is 0x03 (integration time 100ms, repeat rate 500ms)

    while (true) {
       // I2C communcication
       wait(10); // Wakeup time from standby
       ALS_PS_STATUS_REG status; 
        do { 
            status.raw = i2c.read(LTR329ALS01_ADDR_ALS_STATUS);  
        } while(!status.isNewData || status.isInValid); 

        //Reading Lux from 2 channels
        i2c.read(LTR329ALS01_I2C_ADDRESS, LTR329ALS01_ADDR_ALS_DATA_CH_1_0, Data[0]);
        i2c.read(LTR329ALS01_I2C_ADDRESS, LTR329ALS01_ADDR_ALS_DATA_CH_1_1, Data[1]);

        i2c.read(LTR329ALS01_I2C_ADDRESS, LTR329ALS01_ADDR_ALS_DATA_CH_0_0, Data[2]);
        i2c.read(LTR329ALS01_I2C_ADDRESS, LTR329ALS01_ADDR_ALS_DATA_CH_0_1, Data[3]);

        ALS_CH1_ADC_Data = (Data[1] << 8) | Data[0]; // Combining lower and upper bytes to give 16-bit Ch1 data
        ALS_CH0_ADC_Data = (Data[3] << 8) | Data[2]; // Combining lower and upper bytes to give 16-bit Ch0 data

        float ratio = ALS_CH1_ADC_Data / (ALS_CH0_ADC_Data + ALS_CH1_ADC_Data); 
        
        if(ALS_CH0_ADC_Data + ALS_CH1_ADC_Data == 0) return 0; // Avoid to Div by Zero 

        if(ratio < 0.45) {
            lux = (1.7743 * ALS_CH0_ADC_Data + 1.1059 * ALS_CH1_ADC_Data) / ALS_GAIN[gain] / ALS_INT[intTime] / pFactor; 
        }
        else if(ratio < 0.64 &&  ratio >= 0.45) {
            lux = (4.2785 * ALS_CH0_ADC_Data - 1.9548 * ALS_CH1_ADC_Data) / ALS_GAIN[gain] / ALS_INT[intTime] / pFactor; 
        }
        else if(ratio < 0.85 &&  ratio >= 0.64) {
            lux = (0.5926 * ALS_CH0_ADC_Data + 0.1185 * ALS_CH1_ADC_Data) / ALS_GAIN[gain] / ALS_INT[intTime] / pFactor; 
        }
        else {
            lux = 0; 
        }
    }
}