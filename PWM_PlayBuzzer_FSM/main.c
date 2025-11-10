#define PROJECT_CLOCK_FREQUENCY 4000000
#define PROJECT_ONE_MICROSECOND_DELAY 4

#include "ti/devices/msp/msp.h"
#include "../LP_MSPM0G3507/MKII.h"
#include "../LP_MSPM0G3507/timer.h"
#include "../LP_MSPM0G3507/bsp.h"
#include "../LP_MSPM0G3507/tone_pitch.h"
#include "../LP_MSPM0G3507/mspm0g350x_int.h"
#include "FSM_A.h"

typedef enum {up,down} PWM_direction;

//Defines for PWM
#define PWM_PERIOD 10*ONE_MILLISECOND_DELAY //PWM f = 100 Hz => T = ms
#define PWM_DELTA 100*ONE_MICROSECOND_DELAY // 1% Change in duty cycle
#define PWM_PRESCALE 3 
#define TRUE 1
#define FALSE 0

//Function Prototypes
uint32_t CalculatePeriod(uint32_t note);
FSMState NextStateAlgorithm(void *FSM);
void OutputSong(void *FSM);
void Group1CallbackFunction(void *CallbackObject);

//Global variables
Callback_s Group1 = {(void *) 0,Group1CallbackFunction};

//Timer Object Creations
Timer_ClockConfig TimerA1ClockConfig = 
{
    .clockSel = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE,
    .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_1,
    .prescale = PWM_PRESCALE
};
Timer_TimerConfig TimerA1TimerConfig =
{
    .period = PWM_PERIOD,
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
    uint32_t priority = 2;

    //FSM Declaration
    FSMType LED_Pushbutton_FSM = {INACTIVE,INACTIVE,0,40000,400,1,Stop,NextStateAlgorithm,OutputSong};
    Group1.CallbackObject = (void *) &LED_Pushbutton_FSM;

    //Hardware Initializations
    InitializeLaunchpad(PROJECT_CLOCK_FREQUENCY);
    InitializeBoosterpack(PROJECT_CLOCK_FREQUENCY);

    //Timer Initializations
    InitializeTimerClock(TIMA1, &TimerA1ClockConfig);
    InitializeTimerCompare(TIMA1, &TimerA1TimerConfig);
    InitializeTimerPWM(TIMA1, &TimerA1PWMConfig);
    EnableTimer(TIMA1);

    //Pushbutton Interrupt Setup
    __disable_irq();

    BP_S1_PORT->POLARITY15_0 |= BP_S1_POL_FALLING;    // Falling-edge interrupt
    BP_S1_PORT->CPU_INT.ICLR |= BP_S1_MASK;            // Clear interrupt bit
    BP_S1_PORT->CPU_INT.IMASK |= BP_S1_MASK;           // Enable the S1 interrupt.

    BP_S2_PORT->POLARITY15_0 |= BP_S2_POL_FALLING;
    BP_S2_PORT->CPU_INT.ICLR |= BP_S2_MASK;
    BP_S2_PORT->CPU_INT.IMASK |= BP_S2_MASK;

    // Enable NVIC interrupt and set interrupt priority.
    __NVIC_EnableIRQ(BP_S1_INTERRUPT);
    __NVIC_SetPriority(BP_S1_INTERRUPT,priority);
    __NVIC_EnableIRQ(BP_S2_INTERRUPT);
    __NVIC_SetPriority(BP_S2_INTERRUPT, priority);

    __enable_irq();

    //Main

    while(1){
        OutputFunction(&LED_Pushbutton_FSM);
    }
    return 0;
}

#define S1_PRESSED 1
#define S1_RELEASED 0
#define S2_PRESSED 1
#define S2_RELEASED 0

FSMState NextStateAlgorithm(void *FSM)
{
    FSMType * FSM_l = (FSMType *) FSM;
    FSMState NextState = FSM_l->CurrentState;

    switch (FSM_l->CurrentState) {
    case Stop:
        if (FSM_l->CurrentInput_S1 == S1_RELEASED && FSM_l->CurrentInput_S2 == S2_RELEASED){
            NextState = Stop;
        }
        else if (FSM_l->CurrentInput_S1 == S1_RELEASED && FSM_l->CurrentInput_S2 == S2_PRESSED) {
            NextState = Stop;
        }
        else if (FSM_l->CurrentInput_S1 == S1_PRESSED && FSM_l->CurrentInput_S2 == S2_RELEASED) {
            NextState = Start;
        }
        else if (FSM_l->CurrentInput_S1 == S1_PRESSED && FSM_l->CurrentInput_S2 == S2_PRESSED) {
            NextState = Stop;
        }
        break;
    case Start:
        if (FSM_l->CurrentInput_S1 == S1_RELEASED && FSM_l->CurrentInput_S2 == S2_RELEASED){
            NextState = Start;
        }
        else if (FSM_l->CurrentInput_S1 == S1_RELEASED && FSM_l->CurrentInput_S2 == S2_PRESSED) {
            NextState = Stop;
        }
        else if (FSM_l->CurrentInput_S1 == S1_PRESSED && FSM_l->CurrentInput_S2 == S2_RELEASED) {
            NextState = Start;
        }
        else if (FSM_l->CurrentInput_S1 == S1_PRESSED && FSM_l->CurrentInput_S2 == S2_PRESSED) {
            NextState = Stop;
        }
        break;
    }
    return NextState;
}

void OutputSong(void *FSM){
    FSMType * FSM_l = (FSMType *) FSM;
    uint32_t period =  CalculatePeriod(NOTE_C3);
    if (FSM_l->CurrentState == Start){
        TimerA1TimerConfig.period = period;
        TimerA1PWMConfig.duty = period >> 1;
        InitializeTimerCompare(TIMA1, &TimerA1TimerConfig);
        InitializeTimerPWM(TIMA1, &TimerA1PWMConfig);
    }
    if(FSM_l->CurrentState == Stop){
        TimerA1PWMConfig.duty = 0;
        InitializeTimerPWM(TIMA1, &TimerA1PWMConfig);
    }
}

void Group1CallbackFunction(void *CallbackObject){
    FSMType *FSM = (FSMType *)CallbackObject;
    OutputSong((void *)FSM);

    uint32_t IIDXValue = BP_S1_PORT->CPU_INT.IIDX;

    if (IIDXValue == (BP_S1_PIN+1)) {
        FSM->CurrentInput_S1 = 1;
    }
    if (IIDXValue == (BP_S2_PIN+1)) {
        FSM->CurrentInput_S2 = 1;
    }
    FSM->CurrentState = NextStateAlgorithm((void *)FSM);
    FSM->CurrentInput_S1 = 0;
    FSM->CurrentInput_S2 = 0;
}

uint32_t CalculatePeriod(uint32_t note){
    double period = (double) PROJECT_CLOCK_FREQUENCY / ((TimerA1ClockConfig.prescale + 1.0) * (TimerA1ClockConfig.divideRatio + 1.0) * note);
    return (uint32_t)(period);
}