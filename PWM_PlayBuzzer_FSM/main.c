#define PROJECT_CLOCK_FREQUENCY 4000000
#define PROJECT_ONE_MICROSECOND_DELAY 4

#include "../LP_MSPM0G3507/MKII.h"
#include "../LP_MSPM0G3507/bsp.h"
#include "../LP_MSPM0G3507/mspm0g350x_int.h"
#include "../LP_MSPM0G3507/timer.h"
#include "../LP_MSPM0G3507/tone_pitch.h"
#include "FSM_A.h"
#include "ti/devices/msp/msp.h"

typedef enum { up, down } PWM_direction;

#define NT NOTE_C4

// Defines for PWM
#define PWM_PERIOD CLOCK_FREQUENCY / (8 * NT) // PWM f = 100 Hz => T = ms
#define PWM_DELTA 100 * ONE_MICROSECOND_DELAY // 1% Change in duty cycle
#define PWM_PRESCALE 0
#define TRUE 1
#define FALSE 0
#define NOTES_DELAY (1000 * ONE_MILLISECOND_DELAY)
#define WHOLE_NOTE (1000 * ONE_MILLISECOND_DELAY)

// Function Prototypes
uint32_t CalculatePeriod(uint32_t note);
FSMState NextStateAlgorithm(void *FSM);
void PlaySong(void *FSM);
void FSMSong(void *FSM);
void Group1CallbackFunction(void *CallbackObject);
void TimerG8CallbackFunction(void *CallbackObject);

// Global variables
Callback_s Group1 = {(void *)0, Group1CallbackFunction};

// notes in the melody:
int melody[] = {NOTE_C4_1, NOTE_C4, NOTE_D4,  NOTE_C4,  NOTE_F4, NOTE_E4,
                NOTE_C4_1, NOTE_C4, NOTE_D4,  NOTE_C4,  NOTE_G4, NOTE_F4,
                NOTE_C4_1, NOTE_C4, NOTE_C5,  NOTE_A4,  NOTE_F4, NOTE_F4,
                NOTE_E4,   NOTE_D4, NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4,
                NOTE_G4,   NOTE_F4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4, 4, 2, 2, 2, 1, 4, 4, 2, 2, 2, 1, 4,
                       4, 2, 2, 4, 4, 2, 1, 4, 4, 2, 2, 2, 1};

int currentNote = 0;

// Timer Object Creations
Timer_ClockConfig TimerA1ClockConfig = {
    .clockSel = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE,
    .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_8,
    .prescale = PWM_PRESCALE};
Timer_TimerConfig TimerA1TimerConfig = {
    .period = 0,
    .timerMode = (GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1)};
Timer_PWMConfig TimerA1PWMConfig = {
    .index = BUZZER_INDEX,
    .iomuxMode = (IOMUX_PINCM_PC_CONNECTED | IOMUX_MODE4),
    .ccr = 0,
    .ccpd = GPTIMER_CCPD_C0CCP0_OUTPUT,
    .ccctl = GPTIMER_CCCTL_01_COC_COMPARE,
    .octl = GPTIMER_OCTL_01_CCPO_FUNCVAL,
    .ccact = (GPTIMER_CCACT_01_CDACT_CCP_LOW | GPTIMER_CCACT_01_LACT_CCP_HIGH),
    .duty = 0};

// Use Timer 8 used for time delay between pitches.
Callback_s TimerG8 = {(void *)0, TimerG8CallbackFunction};

Timer_ClockConfig TimerG8ClockConfig = {
    .clockSel = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE,
    .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_1,
    .prescale = 0};
Timer_TimerConfig TimerG8TimerConfig = {
    .period = NOTES_DELAY,
    .timerMode = (GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_0)};
// Timer_InterruptConfig TimerG8InterruptConfig = {.imask =
//                                                     GPTIMER_CPU_INT_IMASK_Z_SET,
//                                                 .priority = 1,
//                                                 .IRQnum = TIMG8_INT_IRQn};

int main(void) {
  uint32_t priority = 2;

  // FSM Declaration
  FSMType Song_FSM = {INACTIVE, INACTIVE,           0,       40000, 400, 1,
                      Stop,     NextStateAlgorithm, PlaySong};
  Group1.CallbackObject = (void *)&Song_FSM;

  // Hardware Initializations
  InitializeLaunchpad(PROJECT_CLOCK_FREQUENCY);
  InitializeBoosterpack(PROJECT_CLOCK_FREQUENCY);

  uint32_t period = CalculatePeriod(NT);
  TimerA1TimerConfig.period = period;

  // Timer Initializations
  InitializeTimerClock(TIMA1, &TimerA1ClockConfig);
  InitializeTimerCompare(TIMA1, &TimerA1TimerConfig);
  InitializeTimerPWM(TIMA1, &TimerA1PWMConfig);
  EnableTimer(TIMA1);

  // Pushbutton Interrupt Setup
  __disable_irq();

  BP_S1_PORT->POLARITY15_0 |= BP_S1_POL_FALLING; // Falling-edge interrupt
  BP_S1_PORT->CPU_INT.ICLR |= BP_S1_MASK;        // Clear interrupt bit
  BP_S1_PORT->CPU_INT.IMASK |= BP_S1_MASK;       // Enable the S1 interrupt.

  BP_S2_PORT->POLARITY15_0 |= BP_S2_POL_FALLING;
  BP_S2_PORT->CPU_INT.ICLR |= BP_S2_MASK;
  BP_S2_PORT->CPU_INT.IMASK |= BP_S2_MASK;

  // Enable NVIC interrupt and set interrupt priority.
  __NVIC_EnableIRQ(BP_S1_INTERRUPT);
  __NVIC_SetPriority(BP_S1_INTERRUPT, priority);
  __NVIC_EnableIRQ(BP_S2_INTERRUPT);
  __NVIC_SetPriority(BP_S2_INTERRUPT, priority);

  TimerG8.CallbackObject = (void *)&Song_FSM;
  InitializeTimerClock(TIMG8, &TimerG8ClockConfig);
  InitializeTimerCompare(TIMG8, &TimerG8TimerConfig);
  //   InitializeTimerInterrupt(TIMG8, &TimerG8InterruptConfig);
//   EnableTimer(TIMG8);

  __enable_irq();

  // Main

  while (1) {
    __WFI();
    OutputFunction(&Song_FSM);
  }
  return 0;
}

void PlaySong(void *FSM) {
  FSMType *FSM_l = (FSMType *)FSM;

  if (FSM_l->CurrentState == Start) {
    while (currentNote < sizeof(melody)) {
      uint32_t period = CalculatePeriod(melody[currentNote]);

      // Update Note
      TimerA1TimerConfig.period = period;
      TimerA1PWMConfig.duty = period >> 1;
    //   UpdateDutyCycle(TIMA1, &TimerA1PWMConfig);

      // Duration of Note
      // to calculate the note duration, take one second
      // divided by the note type.
      // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int duration = WHOLE_NOTE / noteDurations[currentNote];
      TimerG8TimerConfig.period = duration + NOTES_DELAY; // delay between
      TimeDelay(TIMG8, &TimerG8TimerConfig);
      currentNote++;

    }
    currentNote = 0;
  }

  if (FSM_l->CurrentState == Stop) {
    TimerA1PWMConfig.duty = 0;
    UpdateDutyCycle(TIMA1, &TimerA1PWMConfig);
  }
}

// void FSMSong(void *FSM) {
//   FSMType *FSM_l = (FSMType *)FSM;

//   // Check if num of pitches and durations are out of match; do nothing if
//   not
//   //   if (sizeof(melody) != sizeof(noteDurations))
//   //     return;

//   // Iterate through song array
//   if (FSM_l->CurrentState == Start) {
//     PlaySong();
//   }
//   if (FSM_l->CurrentState == Stop) {
//     TimerA1PWMConfig.duty = 0;
//     UpdateDutyCycle(TIMA1, &TimerA1PWMConfig);
//   }
// }

void TimerG8CallbackFunction(void *CallbackObject) {
  FSMType *FSM = (FSMType *)CallbackObject;
  if (TIMG8->CPU_INT.IIDX == GPTIMER_CPU_INT_IIDX_STAT_Z) {
    // Stop Current Note
    TimerA1PWMConfig.duty = 0;
    UpdateDutyCycle(TIMA1, &TimerA1PWMConfig);

    DisableTimer(TIMG8);
    currentNote++;

    // Check if song is over
    if (currentNote == (sizeof(melody) - 1)) {
      currentNote = 0; // Reset
      FSM->CurrentState = Stop;
    } else {
      PlaySong((void *)FSM);
    }
  }
}

uint32_t CalculatePeriod(uint32_t note) {
  double period = (double)PROJECT_CLOCK_FREQUENCY /
                  ((TimerA1ClockConfig.prescale + 1.0) *
                   (TimerA1ClockConfig.divideRatio + 1.0) * note);
  return (uint32_t)(period);
}

//==============
//  FSM State
//==============

#define S1_PRESSED 1
#define S1_RELEASED 0
#define S2_PRESSED 1
#define S2_RELEASED 0

FSMState NextStateAlgorithm(void *FSM) {
  FSMType *FSM_l = (FSMType *)FSM;
  FSMState NextState = FSM_l->CurrentState;

  switch (FSM_l->CurrentState) {
  case Stop:
    if (FSM_l->CurrentInput_S1 == S1_RELEASED &&
        FSM_l->CurrentInput_S2 == S2_RELEASED) {
      NextState = Stop;
    } else if (FSM_l->CurrentInput_S1 == S1_RELEASED &&
               FSM_l->CurrentInput_S2 == S2_PRESSED) {
      NextState = Stop;
    } else if (FSM_l->CurrentInput_S1 == S1_PRESSED &&
               FSM_l->CurrentInput_S2 == S2_RELEASED) {
      NextState = Start;
    } else if (FSM_l->CurrentInput_S1 == S1_PRESSED &&
               FSM_l->CurrentInput_S2 == S2_PRESSED) {
      NextState = Stop;
    }
    break;
  case Start:
    if (FSM_l->CurrentInput_S1 == S1_RELEASED &&
        FSM_l->CurrentInput_S2 == S2_RELEASED) {
      NextState = Start;
    } else if (FSM_l->CurrentInput_S1 == S1_RELEASED &&
               FSM_l->CurrentInput_S2 == S2_PRESSED) {
      NextState = Stop;
    } else if (FSM_l->CurrentInput_S1 == S1_PRESSED &&
               FSM_l->CurrentInput_S2 == S2_RELEASED) {
      NextState = Start;
    } else if (FSM_l->CurrentInput_S1 == S1_PRESSED &&
               FSM_l->CurrentInput_S2 == S2_PRESSED) {
      NextState = Stop;
    }
    break;
  }
  return NextState;
}

void Group1CallbackFunction(void *CallbackObject) {
  FSMType *FSM = (FSMType *)CallbackObject;

  uint32_t IIDXValue = BP_S1_PORT->CPU_INT.IIDX;

  if (IIDXValue == (BP_S1_PIN + 1)) {
    FSM->CurrentInput_S1 = 1;
  }
  if (IIDXValue == (BP_S2_PIN + 1)) {
    FSM->CurrentInput_S2 = 1;
  }
  FSM->CurrentState = NextStateAlgorithm((void *)FSM);
  FSM->CurrentInput_S1 = 0;
  FSM->CurrentInput_S2 = 0;
}