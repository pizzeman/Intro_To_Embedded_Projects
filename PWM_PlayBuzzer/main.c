#define PROJECT_CLOCK_FREQUENCY 4000000
#define PROJECT_ONE_MICROSECOND_DELAY 4

#include "ti/devices/msp/msp.h"
#include "../LP_MSPM0G3507/timer.h"
#include "../LP_MSPM0G3507/bsp.h"
#include "../LP_MSPM0G3507/tone_pitch.h"
#include "../LP_MSPM0G3507/MKII.h"

//Defines for PWM
#define PWM_PERIOD 10*ONE_MILLISECOND_DELAY //PWM f = 100 Hz => T = ms
#define PWM_DELTA 100*ONE_MICROSECOND_DELAY // 1% Change in duty cycle
#define PWM_PRESCALE 0 
#define TRUE 1
#define FALSE 0

//Fn Prototypes
uint32_t CalculatePeriod(uint32_t note);

//Timer Object Creations
Timer_ClockConfig TimerA1ClockConfig = 
{
    .clockSel = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE,
    .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_1,
    .prescale = PWM_PRESCALE
};
Timer_TimerConfig TimerA1TimerConfig =
{
    .period = 0,
    .timerMode = (GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1)
};
Timer_PWMConfig TimerA1PWMConfig = 
{
    .index = BUZZER_INDEX,
    .iomuxMode = (IOMUX_PINCM_PC_CONNECTED | IOMUX_MODE4),
    .ccr = 0,
    .ccpd = GPTIMER_CCPD_C0CCP0_OUTPUT,
    .ccctl = GPTIMER_CCCTL_01_COC_COMPARE,
    .octl = GPTIMER_OCTL_01_CCPO_FUNCVAL,
    .ccact = (GPTIMER_CCACT_01_CDACT_CCP_LOW | GPTIMER_CCACT_01_LACT_CCP_HIGH),
    .duty = 0
};
int main(void){
    InitializeLaunchpad(PROJECT_CLOCK_FREQUENCY);
    InitializeBoosterpack(PROJECT_CLOCK_FREQUENCY);

    uint32_t period = CalculatePeriod(NOTE_C3);

    TimerA1TimerConfig.period = period;
    TimerA1PWMConfig.duty = period >> 1;

    InitializeTimerClock(TIMA1, &TimerA1ClockConfig);
    InitializeTimerCompare(TIMA1, &TimerA1TimerConfig);
    InitializeTimerPWM(TIMA1, &TimerA1PWMConfig);
    EnableTimer(TIMA1);

    
    return 0;
}

uint32_t CalculatePeriod(uint32_t note){
    double period = (double)PROJECT_CLOCK_FREQUENCY / ((TimerA1ClockConfig.prescale + 1.0) * (TimerA1ClockConfig.divideRatio + 1.0) * note);
    return (uint32_t)(period);
}