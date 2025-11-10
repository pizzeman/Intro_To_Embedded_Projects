#define PROJECT_CLOCK_FREQUENCY 4000000
#define PROJECT_ONE_MICROSECOND_DELAY 4

#include "ti/devices/msp/msp.h"
#include "LP_MSPM0G3507/MKII.h"

#define FALSE 0
#define TRUE 1

int main(void)
{

  // Initialization section: MKII BoosterPack and variables.
  InitializeBoosterpack(32000000);

  uint32_t S1 = 0;
  uint32_t S2 = 0;

  // Infinite loop
  while (TRUE) {
    S1 = BP_ReadS1();
    S2 = BP_ReadS2();

    while (S1) {
      S1 = BP_ReadS1();
      if (!S1) {
        BP_ToggleBlueLED();
      }
    }

    while (S2) {
      S2 = BP_ReadS2();
      if (!S2) {
        BP_ToggleRedLED();
      }
    }
    S1 = 0;
    S2 = 0;

    ClockDelay(250 * ONE_MILLISECOND_DELAY);
  }
  return 0;
}