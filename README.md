# LightSensorDriver

Library for the light sensor LTR329ALS01

## Example

```c
#include "mbed.h"
#include "LTR329ALS.h"

I2C i2c(I2C_SDA, I2C_SCL);

LTR329ALS light(&i2c, 0x29 << 1);

int main()
{
    while (1) 
    {
        ThisThread::sleep_for(500ms);
        
        printf("Lux:", int(light.readLux()));
    }
}

```
