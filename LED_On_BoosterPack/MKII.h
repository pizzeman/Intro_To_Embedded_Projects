#ifndef MKII_H_
#define MKII_H_

#include "bsp.h"

// Complete the following #define statements

// Push Button S1 J4.33
#define BP_S1_INDEX 
#define BP_S1_PIN 
#define BP_S1_MASK 
#define BP_S1_PORT 

// Push Button S2 J4.32
#define BP_S2_INDEX 
#define BP_S2_PIN 
#define BP_S2_MASK 
#define BP_S2_PORT 

// LED Red J4.39
#define BP_RGB_R_PIN 
#define BP_RGB_R_MASK 
#define BP_RGB_R_PORT 
#define BP_RGB_R_INDEX 

// LED Green J4.38
#define BP_RGB_G_PIN 
#define BP_RGB_G_MASK 
#define BP_RGB_G_PORT 
#define BP_RGB_G_INDEX 

// LED Blue J4.37
#define BP_RGB_B_PIN 
#define BP_RGB_B_MASK 
#define BP_RGB_B_PORT 
#define BP_RGB_B_INDEX 

void InitializeBoosterpack(uint32_t ClockFrequency);

/**
 * @brief Read S1 from the Boosterpack.
 * On J22 jumper on the Launchpad, shunt pins 2 and 3 (see LP-MSPM0G3507 schematic)
 * to connect pin PA11 to the BoosterPack
 * @param none
 * @return 1 if S1 is pressed, 0 otherwise
 */
uint32_t BP_ReadS1();

/**
 * @brief Read S2 from the Boosterpack.
 * @param none
 * @return 1 if S2 is pressed, 0 otherwise
 */
uint32_t BP_ReadS2();

/**
 * @brief Turn on RGB red LED (D1 on BP).
 * @param none
 * @return none
 */
void BP_TurnOnRedLED(void);

/**
 * @brief Turn off RGB red LED (D1 on BP).
 * @param none
 * @return none
 */
void BP_TurnOffRedLED(void);
/**
 * @brief Toggle RGB red LED (D1 on BP).
 * @param none
 * @return none
 */
void BP_ToggleRedLED(void);

/**
 * @brief Turn on RGB green LED (D1 on BP).
 * @param none
 * @return none
 */
void BP_TurnOnGreenLED(void);

/**
 * @brief Turn off RGB green LED (D1 on BP).
 * @param none
 * @return none
 */
void BP_TurnOffGreenLED(void);
/**
 * @brief Toggle RGB green LED (D1 on BP).
 * @param none
 * @return none
 */
void BP_ToggleGreenLED(void);

/**
 * @brief Turn on RGB blue LED (D1 on BP).
 * @param none
 * @return none
 */
void BP_TurnOnBlueLED(void);

/**
 * @brief Turn off RGB blue LED (D1 on BP).
 * @param none
 * @return none
 */
void BP_TurnOffBlueLED(void);
/**
 * @brief Toggle RGB blue LED (D1 on BP).
 * @param none
 * @return none
 */
void BP_ToggleBlueLED(void);

#endif
