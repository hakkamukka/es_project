// ----------------------------------------
// Filename: CRG.h
// Description: Routines for setting up the
//   clock and reset generator
// Author: PMcL
// Date: 16-Mar-06

#ifndef CRG_H
#define CRG_H

// new types
#include "types.h"
#include "clock.h"
#include "EEPROM.h"
#include <mc9s12a256.h>     /* derivative information */


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

BOOL CRG_SetupPLL(const UINT32 busClk, const UINT32 oscClk, const UINT32 refClk);

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

BOOL CRG_SetupCOP(const UINT8 watchDogRate);

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

void CRG_SetupRTI(const UINT8 prescaleRate, const UINT8 modulusCount);

#endif