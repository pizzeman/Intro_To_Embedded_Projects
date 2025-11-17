#include <ti/devices/msp/msp.h>
#include "../LP_MSPM0G3507/MKII.h"
#include "../LP_MSPM0G3507/i2c.h"
#include "../LP_MSPM0G3507/SPI.h"
#include "../LP_MSPM0G3507/tmp006.h"
#include "../LP_MSPM0G3507/LCD.h"

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

    LCD_Init();
    LCD_FillScreen(LCD_BLACK);
    LCD_SetTextColor(LCD_CYAN);
    LCD_OutString("AJ \"Pizzeman\" Amos \nand Isaiah \"Stingray\" \nBumgardner\n");   
    char buffer[50];

    while (1) {
      // Wait for ACK
      
      // Use I2C to read sensor value
      int16_t raw_v = sensorTmp006Read(I2C1, TMP006_ADDRESS + VOLTAGE_REGISTER_ADDRESS);
      int16_t raw_t = sensorTmp006Read(I2C1, TMP006_ADDRESS + LOCAL_TEMP_REGISTER_ADDRESS);

      float calc_temp = sensorTmp006ConvertTemp(raw_v, raw_t);

      // Display value on LCD using SPI
      sprintf(buffer, "%.3f", calc_temp); // Convert to char *
      LCD_OutString(buffer);
      LCD_OutString("\n");
      LCD_SetCursor(0, 0);
    }
    return 0;
}