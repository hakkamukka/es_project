// ----------------------------------------
// Filename: CRG.c
// Description: Routines for setting up the
//   clock and reset generator
// Author: PMcL
//         Danny Sok
//         Edward Khou
// Date Created: 24-08-2011
// Date Last Modified: 30-08-2011

#include "CRG.h"


// ----------------------------------------
// CRG_SetupPLL
// 
// Sets up the PLL to generate a certain bus clock
// Input:
//   busClk is the desired bus clock rate in Hz,
//   oscClk is the oscillator clock in Hz, 
//   refClk is the reference clock in Hz
// Output:
//   TRUE if the bus clock was setup successfully
// Conditions:
//   Assumes that refClk divides oscClk evenly
//   Assumes that refClk divides busClk evenly
BOOL CRG_SetupPLL(const UINT32 busClk, const UINT32 oscClk, const UINT32 refClk)
{
  UINT16 loopCounter = 0;
  // We want busClk = 24Mhz
  // oscClk/sysClk = 48Mhz after PPL
  SYNR          = ((busClk / refClk) - 1);
  REFDV         = ((oscClk / refClk) - 1);
  
  PLLCTL_PLLON  = 1;                          // Needs to be written before PPLSEL = 1; Turns no the circuit
  
  // refer to page 33. 50000 is the check window
  while (CRGFLG_LOCK == 0 && loopCounter < 50000) // Running in correct requency
    loopCounter++;
  
  if (loopCounter == 50000)
    return bFALSE;

  CLKSEL_PLLSEL = 1;                           // Sets up PPL
  return bTRUE; 
}

// ----------------------------------------
// CRG_SetupCOP
// 
// Sets up the COP to reset within a certain
//   number of milliseconds
// Input:
//   Desired COP watchdog rate, corresponding to
//   Table 3.3 in the CRG Block User Guide
// Output:
//   TRUE if the COP was setup successfully
// Conditions:
//   none
BOOL CRG_SetupCOP(const UINT8 watchDogRate)
{  
   // Cannot exceed 7 in binary; Cannot be disabled
   if ( (watchDogRate > 7) && (COPCTL_CR == 0) )
    return bFALSE;
   
   COPCTL_RSBCK = 1;                          // To allow for background debugging
   COPCTL_CR    = watchDogRate;
   return bTRUE;
}

// ----------------------------------------
// CRG_SetupRTI
// 
// Sets up the RTI as a periodic timer
// Input:
//   Desired prescale rate and modulus count,
//   corresponding to Table 3.2 in the CRG Block User Guide
// Output:
//   TRUE if the RTI was setup successfully
// Conditions:
//   none

void CRG_SetupRTI(const UINT8 prescaleRate, const UINT8 modulusCount)
{
  // 2^16 = 65353. So RTR[6:4] (Prescale) = 111 and RTR[3:0] (Mod) = 1111.

  RTICTL = (prescaleRate << 4) | modulusCount;
  CRGINT_RTIE = 1;      // enable RTI interupt
}

void interrupt 7 CRG_RTI_ISR(void)
{
  CRGFLG_RTIF = 1;             // Clears the flag stupidly... needs to be written 1.
  
  if (Debug)
    PTT_PTT5 ^= 1;             // Set as output
  
  Clock_MicroSeconds += 65535; // Increase MicroSeconds by 65535 as that's how often this interrupt should be called
                               // 65.535 ms  
  
}
