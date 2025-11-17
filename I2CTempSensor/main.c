#include <ti/devices/msp/msp.h>
#include "../LP_MSPM0G3507/MKII.h"
#include "../LP_MSPM0G3507/i2c.h"
#include "../LP_MSPM0G3507/SPI.h"
#include "../LP_MSPM0G3507/tmp006.h"

// I2C configuration
I2C_Config i2c1_config =
{
 .SCL_index = SCL_PINCM,
 .SDA_index = SDA_PINCM,
 .ClockFrequency = CLOCK_FREQUENCY
};

int main(void)
{
    InitializeBoosterpack(CLOCK_FREQUENCY);
    InitializeI2C(I2C1,&i2c1_config);

    while (1) {
      // Wait for ACK
      
      // Use I2C to read sensor value
      int16_t raw_v = sensorTmp006Read(&i2c1_config, TMP006_ADDRESS | VOLTAGE_REGISTER_ADDRESS);
      int16_t raw_t = sensorTmp006Read(&i2c1_config, TMP006_ADDRESS | LOCAL_TEMP_REGISTER_ADDRESS);

      sensorTmp006ConvertTemp(raw_v, raw_t);
      // Call computation in software

      // Display value on LCD using SPI
    }
    return 0;
}