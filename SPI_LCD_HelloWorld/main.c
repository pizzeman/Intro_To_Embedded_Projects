#include <ti/devices/msp/msp.h>
#include "../LP_MSPM0G3507/bsp.h"
#include "../LP_MSPM0G3507/MKII.h"
#include "../LP_MSPM0G3507/LCD.h"

#define FALSE 0
#define TRUE 1

int main(void)
{
    // Initialize MKII BoosterPack
    InitializeBoosterpack(CLOCK_FREQUENCY);

    LCD_Init();
    LCD_FillScreen(LCD_BLACK);
    LCD_SetTextColor(LCD_CYAN);
    LCD_OutString("AJ \"Pizzeman\" Amos \nand Isaiah \"Stingray\" \nBumgardner\n");     

    while (TRUE) {
        //BP_ToggleBlueLED();
        ClockDelay(250*ONE_MILLISECOND_DELAY);
    }
    return 0;
} 