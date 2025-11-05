#include "MKII.h"

void InitializeBoosterpack(uint32_t ClockFrequency)
{

  ActivatePortAandB();

  // IOMUX configuration for RGB LED on BoosterPack.
  // Red: Peripheral connected and IOMUX mode 1
  // Green: Peripheral connected and IOMUX mode 1
  // Blue: Peripheral connected and IOMUX mode 1

  // IOMUX configuration for S1 and S2 on BoosterPack.
  // S1: Peripheral connected, input enabled, internal resistor enabled, IOMUX mode 1
  // S2: Peripheral connected, input enabled, internal resistor enabled, IOMUX mode 1

  // Enable RGB LED pins as outputs using Data Output Enable (DOE) register

  // Turn off red RGB LED
  // Turn off green RGB LED
  // Turn off blue RGB LED

  // Set the clock frequency
  switch (ClockFrequency) {
  case 40000000: ClockInitialization_40MHz(); break;
  case 80000000: ClockInitialization_80MHz(0); break; // disable clk_out
  default: ClockInitialization_4MHz_32MHz(ClockFrequency);
  }
}

uint32_t BP_ReadS1()
{
}

uint32_t BP_ReadS2()
{
}

void BP_TurnOnRedLED(void)
{

}

void BP_TurnOffRedLED(void)
{

}

void BP_ToggleRedLED(void)
{

}

void BP_TurnOnGreenLED(void)
{

}

void BP_TurnOffGreenLED(void)
{

}

void BP_ToggleGreenLED(void)
{

}

void BP_TurnOnBlueLED(void)
{

}

void BP_TurnOffBlueLED(void)
{

}

void BP_ToggleBlueLED(void)
{
 
}
