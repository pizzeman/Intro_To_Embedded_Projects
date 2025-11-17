#include <ti/devices/msp/msp.h>
#include "../LP_MSPM0G3507/MKII.h"
#include "../LP_MSPM0G3507/i2c.h"

// I2C configuration
I2C_Config i2c1_config =
{
 .SCL_index = SCL_PINCM_index_from_datasheet,
 .SDA_index = SDA_PINCM_index_from_datasheet,
 .ClockFrequency = CLOCK_FREQUENCY
};

int main(void)
{
    InitializeBoosterPack(CLOCK_FREQUENCY);
    InitializeI2C(I2C1,&i2c1_config);

    while (1) {
      
      // Application code to read sensor values,
      // compute temperature, and display on LCD
    }
    return 0;
}