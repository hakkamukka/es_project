// ----------------------------------------
// Filename: timer.c
// Description:
//   Routines to implement general purpose timers
//   Routines to support the modulus down-counter
//     as a periodic timer
// Author: Danny Sok
//         Edward Khou
// Date Created: 24-08-2011
// Date Last Modified: 30-08-2011

// new types
#include "timer.h"

void Timer_SetupPulseAccumulators(void)
{
  PACTL_PAEN = 0; // Pulse Accumulator A System Enable : 0 DISABLE
                  // 8-Bit
                  
  PBCTL_PBEN = 0; // Pulse Accumulator B System Enable : 0 DISABLE
                  // 8-Bit
  
  DLYCT = 0;      // Delayed o/p response disabled
  
  // All 4 8-bit pulse accumulators enabled
  ICPAR_PA0EN = 1;
  ICPAR_PA1EN = 1;
  ICPAR_PA2EN = 1;
  ICPAR_PA3EN = 1;
  // 16-Bit Pulse Accumulator A Control Register
  PACTL_PAEN  = 1; // Pulse Accumulator A System Enable: 1 = ENABLED
  PACTL_PAMOD = 0; // Pulse Accumulator Mode:            0 = event counter mode
  PACTL_PEDGE = 1; // Pulse Accumulator Edge Control:    1 = rissing edges on 
                   // PT7 pin cause the count to be incremented
  PACTL_CLK1  = 0; // Clock Select Bits
  PACTL_CLK0  = 0; // 0 0 = Use timer prescaler clock as timer counter clock
  PACTL_PAOVI = 0; // Pulse Accumulator A Overflow Interrupt enable: 0 = disabled
  PACTL_PAI   = 0; // Pulse Accumulator Input Interrupt enable
  
  // 16-Bit Pulse Accumulator B Control Register
  PBCTL_PBEN  = 0; // Pulse Accumulator B System Enable: 0 = DISABLED
  PBCTL_PBOVI = 0; // Pulse Accumulator B Overflow Interrupt enable: 0 = DISABLED
}

// ----------------------------------------
// Timer_SetupPeriodicTimer
// 
// Sets the period of the periodic timer
// Input:
//   microSeconds is the number of microseconds for one period
//   busClk is the actual bus clock rate in Hz
// Output:
//   none
// Conditions:
//   none

void Timer_SetupPeriodicTimer(const UINT16 microSeconds, const UINT32 busClk)
{
  INT32 second = 1000000;
  //UINT32 cyclePerPeriod = (UINT16) ((microSeconds / 16) * (busClk / second));
  // 24 Mhz bus clock which means 24 million clock cycles per second.
  // so we have 24 cycles per microSecond over a period of 2 milli seconds so
  // 2000 * 24 = 48 000 cycles per period. (0xBB80)
  // We need a 2ms period = 2000 microSeconds
  UINT8 prescale = 16;
  
  MCCTL_MCPR  = 3;   // Increments every (prescaler/busClk) seconds  
  
  MCCTL_ICLAT = 0;   // Input capture latch mode disabled
  MCCTL_RDMCL = 0;   // Reads of MCCNT will return current value
  MCCTL_MODMC = 1;   // Modulus mode enabled
  //MCCTL_MCZI  = 1;   // Modulus counter underflow interrupt enabled
  
  //MCCTL_MCEN  = 1;   // Counter enabled
  
  MCCNT = (UINT16) ((busClk/second) * (microSeconds/prescale));  
  //MCCNT = (UINT16) ((busClk/second) * (microSeconds));  
  MCCTL_FLMC = 1;    // Force load into MCCNT
}

// ----------------------------------------
// Timer_PeriodicTimerEnable
// 
// Enables or disables the periodic timer
// Input:
//   enableTimer is a Boolean value indicating whether to enable the timer
// Output:
//   none
// Conditions:
//   Assumes the timer has been set up

void Timer_PeriodicTimerEnable(const BOOL enable)
{
  MCCTL_MCEN = (UINT8)enable;
  MCCTL_MCZI = (UINT8)enable;  
}

// ----------------------------------------
// Timer_Setup
// 
// Sets up the Enhanced Capture Timer unit for
// operations with the timers
// Input:
//   none
// Output:
//   none
// Conditions:
//   none

void Timer_Setup(void)
{
  //TSCR1_TFFCA   = 0;      // Timer Fast Flag Clear All 0 = Clear normally. Erratum, doesn't work.
  TSCR1_TEN     = 1;      // Timer Enable, 1: ON
  TSCR1_TSFRZ   = 0;      // Timer and Modulus Counter Stop While in Freeze Mode 0: DISABLED           
  TSCR1_TSWAI   = 0;      // Timer Module Stops While in Wait, 0: DISABLED 
  
  DDRT_DDRT4    = 1;      // Set Port T Bit 4 to output for debug mode
  DDRT_DDRT5    = 1;      // Set Port T Bit 5 to output for debug mode
  DDRT_DDRT6    = 1;      // Set Port T Bit 6 to output for debug mode
  
  TSCR2_PR2 = 1;  //prescale 32
  TSCR2_PR1 = 0;
  TSCR2_PR0 = 1;
  
  PTT_PTT4      = 0;      // Mod Down counter 
  PTT_PTT5      = 0;      // RTI
  PTT_PTT6      = 0;      // ECT Channel 7
}

// ----------------------------------------
// Timer_Init
// 
// Sets up a timer channel
// Input:
//   channelNb is the timer channel number
//   aTimerSetup is a structure containing the parameters to 
//     be used in setting up the timer channel:
//       outputCompare is TRUE when the timer should be configured as an
//         output, otherwise it is configured as an input
//       outputAction is the action to take on a successful output compare
//       inputDetection is the type of input capture detection
//       toggleOnOverflow is TRUE when the timer should toggle on overflow
//       interruptEnable is TRUE to enable interrupts
//       pulseAccumulator is TRUE to enable pulse accumulator? Missing typo or meant to be missing?
// Output:
//   none
// Conditions:
//   none

void Timer_Init(const TTimerChannel channelNb, const TTimerSetup * const aTimerSetup)
{
  // TIOS for input/output setup
  // pg. 25 for InputDetection
  // pg. 24 for OutputAction 
  switch(channelNb)
  {
    case TIMER_Ch0:
      TIOS_IOS0 = (UINT8)aTimerSetup->outputCompare;    // 0: Input, 1: Output
            
      switch(aTimerSetup->outputAction)
      {
        case TIMER_OUTPUT_DISCONNECT:
          TCTL2_OM0 = 0;
          TCTL2_OL0 = 0;
        case TIMER_OUTPUT_TOGGLE:
          TCTL2_OM0 = 0;
          
          TCTL2_OL0 = 1;
        case TIMER_OUTPUT_LOW:
          TCTL2_OM0 = 1;
          TCTL2_OL0 = 0;
        case TIMER_OUTPUT_HIGH:
          TCTL2_OM0 = 1;
          TCTL2_OL0 = 1;
      }
      
      //TCTL4_EDG0x = (UINT8)aTimerSetup->inputDetection; 
      switch(aTimerSetup->inputDetection)
      {
        case TIMER_INPUT_OFF:
          TCTL4_EDG0B = 0;
          TCTL4_EDG0A = 0;
        case TIMER_INPUT_RISING:
          TCTL4_EDG0B = 0;
          TCTL4_EDG0A = 1;
        case TIMER_INPUT_FALLING:
          TCTL4_EDG0B = 1;
          TCTL4_EDG0A = 0;
        case TIMER_INPUT_ANY:
          TCTL4_EDG0B = 1;
          TCTL4_EDG0A = 1;            
      }
      
      TTOV_TOV0 = (UINT8)aTimerSetup->toggleOnOverflow; // 0: OFF, 1: ON  
      TIE_C0I   = (UINT8)aTimerSetup->interruptEnable;  // 0: OFF, 1: ON
      break;
    case TIMER_Ch1:
      TIOS_IOS1 = (UINT8)aTimerSetup->outputCompare;    // 0: Input, 1: Output
      
      switch(aTimerSetup->outputAction)
      {
        case TIMER_OUTPUT_DISCONNECT:
          TCTL2_OM1 = 0;
          TCTL2_OL1 = 0;
        case TIMER_OUTPUT_TOGGLE:
          TCTL2_OM1 = 0;
          TCTL2_OL1 = 1;
        case TIMER_OUTPUT_LOW:
          TCTL2_OM1 = 1;
          TCTL2_OL1 = 0;
        case TIMER_OUTPUT_HIGH:
          TCTL2_OM1 = 1;
          TCTL2_OL1 = 1;
      }
      
      //TCTL4_EDG1x = (UINT8)aTimerSetup->inputDetection; 
      switch(aTimerSetup->inputDetection)
      {
        case TIMER_INPUT_OFF:
          TCTL4_EDG1B = 0;
          TCTL4_EDG1A = 0;
        case TIMER_INPUT_RISING:
          TCTL4_EDG1B = 0;
          TCTL4_EDG1A = 1;
        case TIMER_INPUT_FALLING:
          TCTL4_EDG1B = 1;
          TCTL4_EDG1A = 0;
        case TIMER_INPUT_ANY:
          TCTL4_EDG1B = 1;
          TCTL4_EDG1A = 1;
      }
      
      TTOV_TOV1 = (UINT8)aTimerSetup->toggleOnOverflow; // 0: OFF, 1: ON
      TIE_C1I   = (UINT8)aTimerSetup->interruptEnable;  // 0: OFF, 1: ON
      break;
    case TIMER_Ch2:
      TIOS_IOS2 = (UINT8)aTimerSetup->outputCompare;    // 0: Input, 1: Output
      
      switch(aTimerSetup->outputAction)
      {
        case TIMER_OUTPUT_DISCONNECT:
          TCTL2_OM2 = 0;
          TCTL2_OL2 = 0;
        case TIMER_OUTPUT_TOGGLE:
          TCTL2_OM2 = 0;
          TCTL2_OL2 = 1;
        case TIMER_OUTPUT_LOW:
          TCTL2_OM2 = 1;
          TCTL2_OL2 = 0;
        case TIMER_OUTPUT_HIGH:
          TCTL2_OM2 = 1;
          TCTL2_OL2 = 1;
      }
      
      //TCTL4_EDG2x = (UINT8)aTimerSetup->inputDetection; 
      switch(aTimerSetup->inputDetection)
      {
        case TIMER_INPUT_OFF:
          TCTL4_EDG2B = 0;
          TCTL4_EDG2A = 0;
        case TIMER_INPUT_RISING:
          TCTL4_EDG2B = 0;
          TCTL4_EDG2A = 1;
        case TIMER_INPUT_FALLING:
          TCTL4_EDG2B = 1;
          TCTL4_EDG2A = 0;
        case TIMER_INPUT_ANY:
          TCTL4_EDG2B = 1;
          TCTL4_EDG2A = 1;
      }
      
      TTOV_TOV2 = (UINT8)aTimerSetup->toggleOnOverflow; // 0: OFF, 1: ON
      TIE_C2I   = (UINT8)aTimerSetup->interruptEnable;  // 0: OFF, 1: ON
      break;
    case TIMER_Ch3:
      TIOS_IOS3 = (UINT8)aTimerSetup->outputCompare;    // 0: Input, 1: Output
      
      switch(aTimerSetup->outputAction)
      {
        case TIMER_OUTPUT_DISCONNECT:
          TCTL2_OM3 = 0;
          TCTL2_OL3 = 0;
        case TIMER_OUTPUT_TOGGLE:
          TCTL2_OM3 = 0;
          TCTL2_OL3 = 1;
        case TIMER_OUTPUT_LOW:
          TCTL2_OM3 = 1;
          TCTL2_OL3 = 0;
        case TIMER_OUTPUT_HIGH:
          TCTL2_OM3 = 1;
          TCTL2_OL3 = 1;
      }
      
      //TCTL4_EDG3x = (UINT8)aTimerSetup->inputDetection; 
      switch(aTimerSetup->inputDetection)
      {
        case TIMER_INPUT_OFF:
          TCTL4_EDG3B = 0;
          TCTL4_EDG3A = 0;
        case TIMER_INPUT_RISING:
          TCTL4_EDG3B = 0;
          TCTL4_EDG3A = 1;
        case TIMER_INPUT_FALLING:
          TCTL4_EDG3B = 1;
          TCTL4_EDG3A = 0;
        case TIMER_INPUT_ANY:
          TCTL4_EDG3B = 1;
          TCTL4_EDG3A = 1;
      }
      
      TTOV_TOV3 = (UINT8)aTimerSetup->toggleOnOverflow; // 0: OFF, 1: ON
      TIE_C3I   = (UINT8)aTimerSetup->interruptEnable;  // 0: OFF, 1: ON
      break;  
    case TIMER_Ch4:
      TIOS_IOS4 = (UINT8)aTimerSetup->outputCompare;    // 0: Input, 1: Output
      
      switch(aTimerSetup->outputAction)
      {
        case TIMER_OUTPUT_DISCONNECT:
          TCTL1_OM4 = 0;
          TCTL1_OL4 = 0;
        case TIMER_OUTPUT_TOGGLE:
          TCTL1_OM4 = 0;
          TCTL1_OL4 = 1;
        case TIMER_OUTPUT_LOW:
          TCTL1_OM4 = 1;
          TCTL1_OL4 = 0;
        case TIMER_OUTPUT_HIGH:
          TCTL1_OM4 = 1;
          TCTL1_OL4 = 1;
      }
      
      //TCTL3_EDG4x = (UINT8)aTimerSetup->inputDetection; 
      switch(aTimerSetup->inputDetection)
      {
        case TIMER_INPUT_OFF:
          TCTL3_EDG4B = 0;
          TCTL3_EDG4A = 0;
        case TIMER_INPUT_RISING:
          TCTL3_EDG4B = 0;
          TCTL3_EDG4A = 1;
        case TIMER_INPUT_FALLING:
          TCTL3_EDG4B = 1;
          TCTL3_EDG4A = 0;
        case TIMER_INPUT_ANY:
          TCTL3_EDG4B = 1;
          TCTL3_EDG4A = 1;
      }
      
      TTOV_TOV4 = (UINT8)aTimerSetup->toggleOnOverflow; // 0: OFF, 1: ON
      TIE_C4I   = (UINT8)aTimerSetup->interruptEnable;  // 0: OFF, 1: ON
      break;
    case TIMER_Ch5:
      TIOS_IOS5 = (UINT8)aTimerSetup->outputCompare;    // 0: Input, 1: Output
      
      switch(aTimerSetup->outputAction)
      {
        case TIMER_OUTPUT_DISCONNECT:
          TCTL1_OM5 = 0;
          TCTL1_OL5 = 0;
        case TIMER_OUTPUT_TOGGLE:
          TCTL1_OM5 = 0;
          TCTL1_OL5 = 1;
        case TIMER_OUTPUT_LOW:
          TCTL1_OM5 = 1;
          TCTL1_OL5 = 0;
        case TIMER_OUTPUT_HIGH:
          TCTL1_OM5 = 1;
          TCTL1_OL5 = 1;
      }
      
      //TCTL3_EDG5x = (UINT8)aTimerSetup->inputDetection; 
      switch(aTimerSetup->inputDetection)
      {
        case TIMER_INPUT_OFF:
          TCTL3_EDG5B = 0;
          TCTL3_EDG5A = 0;
        case TIMER_INPUT_RISING:
          TCTL3_EDG5B = 0;
          TCTL3_EDG5A = 1;
        case TIMER_INPUT_FALLING:
          TCTL3_EDG5B = 1;
          TCTL3_EDG5A = 0;
        case TIMER_INPUT_ANY:
          TCTL3_EDG5B = 1;
          TCTL3_EDG5A = 1;
      }
      
      TTOV_TOV5 = (UINT8)aTimerSetup->toggleOnOverflow; // 0: OFF, 1: ON
      TIE_C5I   = (UINT8)aTimerSetup->interruptEnable;  // 0: OFF, 1: ON
      break;
    case TIMER_Ch6:
      TIOS_IOS6 = (UINT8)aTimerSetup->outputCompare;    // 0: Input, 1: Output
      
      switch(aTimerSetup->outputAction)
      {
        case TIMER_OUTPUT_DISCONNECT:
          TCTL1_OM6 = 0;
          TCTL1_OL6 = 0;
        case TIMER_OUTPUT_TOGGLE:
          TCTL1_OM6 = 0;
          TCTL1_OL6 = 1;
        case TIMER_OUTPUT_LOW:
          TCTL1_OM6 = 1;
          TCTL1_OL6 = 0;
        case TIMER_OUTPUT_HIGH:
          TCTL1_OM6 = 1;
          TCTL1_OL6 = 1;
      }
      
      //TCTL3_EDG6x = (UINT8)aTimerSetup->inputDetection; 
      switch(aTimerSetup->inputDetection)
      {
        case TIMER_INPUT_OFF:
          TCTL3_EDG6B = 0;
          TCTL3_EDG6A = 0;
        case TIMER_INPUT_RISING:
          TCTL3_EDG6B = 0;
          TCTL3_EDG6A = 1;
        case TIMER_INPUT_FALLING:
          TCTL3_EDG6B = 1;
          TCTL3_EDG6A = 0;
        case TIMER_INPUT_ANY:
          TCTL3_EDG6B = 1;
          TCTL3_EDG6A = 1;
      }
      
      TTOV_TOV6 = (UINT8)aTimerSetup->toggleOnOverflow; // 0: OFF, 1: ON
      TIE_C6I   = (UINT8)aTimerSetup->interruptEnable;  // 0: OFF, 1: ON
      break;
    case TIMER_Ch7:
      TIOS_IOS7 = (UINT8)aTimerSetup->outputCompare;    // 0: Input, 1: Output
      
      switch(aTimerSetup->outputAction)
      {
        case TIMER_OUTPUT_DISCONNECT:
          TCTL1_OM7 = 0;
          TCTL1_OL7 = 0;
        case TIMER_OUTPUT_TOGGLE:
          TCTL1_OM7 = 0;
          TCTL1_OL7 = 1;
        case TIMER_OUTPUT_LOW:
          TCTL1_OM7 = 1;
          TCTL1_OL7 = 0;
        case TIMER_OUTPUT_HIGH:
          TCTL1_OM7 = 1;
          TCTL1_OL7 = 1;
      }
      
      //TCTL3_EDG7x = (UINT8)aTimerSetup->inputDetection; 
      switch(aTimerSetup->inputDetection)
      {
        case TIMER_INPUT_OFF:
          TCTL3_EDG7B = 0;
          TCTL3_EDG7A = 0;
        case TIMER_INPUT_RISING:
          TCTL3_EDG7B = 0;
          TCTL3_EDG7A = 1;
        case TIMER_INPUT_FALLING:
          TCTL3_EDG7B = 1;
          TCTL3_EDG7A = 0;
        case TIMER_INPUT_ANY:
          TCTL3_EDG7B = 1;
          TCTL3_EDG7A = 1;
      }
      
      TTOV_TOV7 = (UINT8)aTimerSetup->toggleOnOverflow; // 0: OFF, 1: ON
      TIE_C7I   = (UINT8)aTimerSetup->interruptEnable;  // 0: OFF, 1: ON  -- ARM BIT
      break;
  }
}

// ----------------------------------------
// Timer_Enable
// 
// Enables or disables a timer channel interrupt
// Input:
//   channelNb is the timer channel number
//   enableInt is a Boolean value indicating whether to enable the 
//     interrupt on the timer channel
// Output:
//   none
// Conditions:
//   none

void Timer_Enable(const TTimerChannel channelNb, const BOOL enableInt)
{
  switch (channelNb)
  {
    case TIMER_Ch0:
      TIE_C0I = (UINT8)enableInt;
      break;
    case TIMER_Ch1:
      TIE_C1I = (UINT8)enableInt;
      break;
    case TIMER_Ch2:
      TIE_C2I = (UINT8)enableInt;
      break;
    case TIMER_Ch3:
      TIE_C3I = (UINT8)enableInt;
      break;
    case TIMER_Ch4:
      TIE_C4I = (UINT8)enableInt;
      break;
    case TIMER_Ch5:
      TIE_C5I = (UINT8)enableInt;
      break;
    case TIMER_Ch6:
      TIE_C6I = (UINT8)enableInt;
      break;
    case TIMER_Ch7:
      TIE_C7I = (UINT8)enableInt;
      break;
  }
}

// ----------------------------------------
// Timer_Enabled
// 
// Returns the status of a timer
// Input:
//   channelNb is the timer channel number
// Output:
//   a Boolean value indicating whether the channel is enabled
// Conditions:
//   none

BOOL Timer_Enabled(const TTimerChannel channelNb)
{
  switch (channelNb)
  {
    case TIMER_Ch0:
      return TIE_C0I;
      break;
    case TIMER_Ch1:
      return TIE_C1I;
      break;
    case TIMER_Ch2:
      return TIE_C2I;
      break;
    case TIMER_Ch3:
      return TIE_C3I;
      break;
    case TIMER_Ch4:
      return TIE_C4I;
      break;
    case TIMER_Ch5:
      return TIE_C5I;
      break;
    case TIMER_Ch6:
      return TIE_C6I;
      break;
    case TIMER_Ch7:
      return TIE_C7I;
      break;
  }
}

// ----------------------------------------
// Timer_Set
// 
// Sets a timer channel to generate an
//   interrupt after a certain number of busClk cycles
// Input:
//   channelNb is the timer channel number
//   busClkCyclesDelay is the number of busClk cycles
//     to wait until an interrupt is triggered
// Output:
//   none
// Conditions:
//   none

void Timer_Set(const TTimerChannel channelNb, const UINT16 busClkCyclesDelay)
{  
  switch (channelNb)
  {
    case TIMER_Ch0:
      TC0 = TCNT + busClkCyclesDelay;
    case TIMER_Ch1:
      TC1 = TCNT + busClkCyclesDelay;
    case TIMER_Ch2:
      TC2 = TCNT + busClkCyclesDelay;
    case TIMER_Ch3:
      TC3 = TCNT + busClkCyclesDelay;
    case TIMER_Ch4:
      TC4 = TCNT + busClkCyclesDelay;
    case TIMER_Ch5:
      TC5 = TCNT + busClkCyclesDelay;
    case TIMER_Ch6:
      TC6 = TCNT + busClkCyclesDelay;
    case TIMER_Ch7:
      TC7 = TCNT + busClkCyclesDelay;
  }  
}