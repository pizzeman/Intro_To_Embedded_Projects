#define PROJECT_CLOCK_FREQUENCY 4000000
#define PROJECT_ONE_MICROSECOND_DELAY 4

#include "../LP_MSPM0G3507/MKII.h"
#include "../LP_MSPM0G3507/bsp.h"
#include "../LP_MSPM0G3507/mspm0g350x_int.h"
#include "../LP_MSPM0G3507/timer.h"
#include "../LP_MSPM0G3507/tone_pitch.h"
#include "FSM_A.h"
#include "ti/devices/msp/msp.h"
#include "../LP_MSPM0G3507/LCD.h"
#include "../LP_MSPM0G3507/SPI.h"


#define NT NOTE_C4

// Defines for PWM
#define PWM_PERIOD CLOCK_FREQUENCY / (8 * NT) // PWM f = 100 Hz => T = ms
#define PWM_DELTA 100 * ONE_MICROSECOND_DELAY // 1% Change in duty cycle
#define PWM_PRESCALE 0
#define NOTES_DELAY (20 * ONE_MILLISECOND_DELAY) // Duration in between every note
#define WHOLE_NOTE (1000 * ONE_MILLISECOND_DELAY)

// Function Prototypes
uint32_t CalculatePeriod(uint32_t note);
FSMState NextStateAlgorithm(void *FSM);
void PlaySong(void *FSM);
void FSMSong(void *FSM);
void Group1CallbackFunction(void *CallbackObject);
char* NoteToText(int note);

// Global variables
Callback_s Group1 = {(void *)0, Group1CallbackFunction};

// notes in the melody:
int melody[] = {NOTE_C4_1,   NOTE_C4, NOTE_D4,  NOTE_C4,  NOTE_F4, NOTE_E4,
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
    .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_1,
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
Timer_ClockConfig TimerG12ClockConfig = {
    .clockSel = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE,
    .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_1,
    .prescale = 0};

Timer_TimerConfig TimerG12TimerConfig = {
    .period = NOTES_DELAY,
    .timerMode = (GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_0)};

int main(void) {
  uint32_t priority = 2;

  // FSM Declaration
  FSMType Song_FSM = {INACTIVE, INACTIVE,           0,       40000, 400, 1,
                      Stop,     NextStateAlgorithm, PlaySong};
  Group1.CallbackObject = (void *)&Song_FSM;

  // Hardware Initializations
  InitializeLaunchpad(PROJECT_CLOCK_FREQUENCY);
  InitializeBoosterpack(PROJECT_CLOCK_FREQUENCY);

  //LCD Initializations
  LCD_Init();
  LCD_FillScreen(LCD_BLACK);
  LCD_SetTextColor(LCD_CYAN);
  LCD_OutString("AJ \"Pizzeman\" Amos \nand Isaiah \"Stingray\" \nBumgardner\n");   
  char buffer[50];

  LCD_OutString(NoteToText(melody[0]));

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

  InitializeTimerClock(TIMG12, &TimerG12ClockConfig);

  __enable_irq();

  // Main

  while (1) {
    OutputFunction(&Song_FSM);
  }
  return 0;
}

//==================================
//           Playing Notes
//==================================

void PlaySong(void *FSM) {
  FSMType *FSM_l = (FSMType *)FSM;

  if (FSM_l->CurrentState == Start) {
    if (currentNote >= (sizeof(melody)/sizeof(melody[0])))
      currentNote = 0; // go back to beginning of song
    uint32_t period = CalculatePeriod(melody[currentNote]);

    // Update Note
    TimerA1TimerConfig.period = period;
    TimerA1PWMConfig.duty = period >> 1;
    TIMA1->COUNTERREGS.LOAD = TimerA1TimerConfig.period;

    // Duration of Note
    // to calculate the note duration, take one second
    // divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = WHOLE_NOTE / noteDurations[currentNote];
    TimerG12TimerConfig.period = duration; // delay between

    UpdateDutyCycle(TIMA1, &TimerA1PWMConfig); // Plays note on speaker
    TimeDelay(TIMG12, &TimerG12TimerConfig); // Sets duration of note

    // Add a delay between notes
    TimerA1PWMConfig.duty = 0;
    TimerG12TimerConfig.period = NOTES_DELAY; // delay between

    UpdateDutyCycle(TIMA1, &TimerA1PWMConfig);
    TimeDelay(TIMG12, &TimerG12TimerConfig);

    currentNote++;
  }

  // Turn set duty cycle to 0 if in stop state
  if (FSM_l->CurrentState == Stop) {
    LCD_SetCursor(0, 0);
    LCD_OutString("THE SONG HAS BEEN PAUSED!");
    LCD_OutString("\n");
    
    TimerA1PWMConfig.duty = 0;
    UpdateDutyCycle(TIMA1, &TimerA1PWMConfig);
    BP_TurnOffBlueLED();
  }
}

uint32_t CalculatePeriod(uint32_t note) {
  double period = (double)PROJECT_CLOCK_FREQUENCY /
                  ((TimerA1ClockConfig.prescale + 1.0) *
                   (TimerA1ClockConfig.divideRatio + 1.0) * note);
  return (uint32_t)(period);
}

//==================================
//            FSM State
//==================================

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

char* NoteToText(int note){
  switch (note){
  default:
    return "PTICH NOT FOUND MF";
  case NOTE_B0:
    return "B0";
  case NOTE_C1:
    return "C1";
  case NOTE_CS1:
    return "C#1";
  case NOTE_D1:
    return "D1";
  case NOTE_DS1:
    return "D#1";
  case NOTE_E1:
    return "E1";
  case NOTE_F1:
    return "F1";
  case NOTE_FS1:
    return "F#1";
  case NOTE_G1:
    return "G1";
  case NOTE_GS1:
    return "G#1";
  case NOTE_A1:
    return "A1";
  case NOTE_AS1:
    return "A#1";
  }
  
}