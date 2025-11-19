#include <ti/devices/msp/msp.h>
#include <stdio.h>
#include "../LP_MSPM0G3507/LCD.h"
#include "../LP_MSPM0G3507/MKII.h"
#include "../LP_MSPM0G3507/SPI.h"
#include "../LP_MSPM0G3507/i2c.h"
#include "../LP_MSPM0G3507/opt3001.h"


// I2C configuration
I2C_Config i2c1_config = {.SCL_index = SCL_PINCM,
                          .SDA_index = SDA_PINCM,
                          .ClockFrequency = CLOCK_FREQUENCY};

int main(void) {
  InitializeBoosterpack(CLOCK_FREQUENCY);
  InitializeI2C(I2C1, &i2c1_config);
  InitializeOpt3001Sensor(I2C1);

  LCD_Init();
  LCD_FillScreen(LCD_BLACK);
  LCD_SetTextColor(LCD_CYAN);
  char buffer[50];

  while (1) {

    int16_t raw_data = sensorOpt3001Read(I2C1, RESULT_REGISTER_ADDRESS);

    float calc_data = sensorOpt3001Convert(raw_data);

    // Display value on LCD using SPI
    sprintf(buffer, "%.3f", calc_data); // Convert to char *
    LCD_OutString(buffer);
    LCD_OutString("\n");
    LCD_SetCursor(0, 0);
  }
  return 0;
}