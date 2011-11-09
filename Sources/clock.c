// ----------------------------------------
// Filename: clock.c
// Description: Routines for maintaining
//   a real-time clock
// Author: PMcL
//         Danny Sok
//         Edward Khou
// Date Created: 12-09-2011
// Date Last Modified: 16-09-2011

// new types
#include "clock.h"




// ----------------------------------------
// Clock_Setup
// 
// Initializes the clock
// Input:
//   prescaleRate is the desired prescale rate and modulus count
//   of the CRG block's real-time interrupt module,
//   corresponding to Table 3.2 in the CRG Block User Guide,
//   to ensure ticks of the clock occur every 65.536 ms
// Output:
//   TRUE if the clock was setup successfully
// Conditions:
//   none

void Clock_Setup(const UINT8 prescaleRate, const UINT8 modulusCount)
{
  // 2^16 = 63536. So RTR[6:4] (Prescale) = 111 and RTR[3:0] (Mod) = 0000.
  Clock_Minutes = 0;
  Clock_Seconds = 0;
  CRG_SetupRTI(prescaleRate, modulusCount);
}

// ----------------------------------------
// Clock_Update
// 
// Updates the clock by converting milliseconds
//   and microseconds into seconds and minutes
// Input:
//   none
// Output:
//   TRUE if clock seconds have changed
// Conditions:
//   Assumes that the clock has been set up

BOOL Clock_Update(void)
{
  INT32 second = 1000000;              // In microSeconds
  INT8  minute = 60;
  
  // Save current state to allow us to modify these sections
  EnterCritical();
   
  // Every half second flash the LED
  if (Clock_MicroSeconds >= second/2)
    PORTE_BIT7 = 0;                // ON
  else
    PORTE_BIT7 = 1;                // OFF
  
  // Update the clock  
  if (Clock_MicroSeconds >= second)
  {
    Clock_Seconds++;
    Clock_MicroSeconds -= second;  // 1 second has passed
    
    if (Clock_Seconds >= minute)
    {
    Clock_Minutes++;               // 1 minute has passed
    Clock_Seconds -= minute;
    }
    
    // Restore state
    ExitCritical();
    return bTRUE;
  }
  
  // Restore state
  ExitCritical();
  return bFALSE;  
}