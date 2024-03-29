// ----------------------------------------
// Filename: SCI.h
// Description: I/O routines for MC9S12 serial
//   communication interface 
// Author: PMcL
// Date: 13-Mar-06

#ifndef SCI_H
#define SCI_H

// new types
#include "types.h"
#include "FIFO.h"
#include "timer.h"
#include "EEPROM.h"
#include <mc9s12a256.h>     /* derivative information */

// ----------------------------------------
// SCI_Setup
// 
// Sets up the Serial Communication Interface
// Input:
//   baudRate is the baud rate in bits/sec
//   busClk is the bus clock rate in Hz
// Output:
//   none
// Conditions:
//   none

void SCI_Setup(const UINT32 baudRate, const UINT32 busClk);
 
// ----------------------------------------
// SCI_InChar
// 
// Get a character from the receive FIFO if it is not empty
// Input:
//   dataPtr is a pointer to memory to store the retrieved byte
// Output:
//   TRUE if the receive FIFO returned a character
// Conditions:
//   Assumes the receive FIFO has been initialized

BOOL SCI_InChar(UINT8 * const dataPtr);  
 
// ----------------------------------------
// SCI_OutChar
//
// Put a byte in the transmit FIFO if it is not full
// Input:
//   data is a byte to be placed in FIFO
// Output:
//   TRUE if the data was placed in the FIFO
// Conditions:
//   Assumes receive FIFO has been initialized

BOOL SCI_OutChar(const UINT8 data);

// ----------------------------------------
// SendData
//
// Place a byte into the SCI0DRL register
// Input:
//   None.
// Output:
//   TRUE if the data was placed in the register
//   FALSE if there is no data to place or unable to place
// Conditions:
//   Assumes receive FIFO has been initialized
BOOL SendData(void);

// ----------------------------------------
// SCI_Poll
//
// Poll the SCI status to see whether to receive
//   or transmit a single byte
// Input:
//   none
// Output:
//   none
// Conditions:
//   Assumes transmit and receive FIFOs have been initialized

void SCI_Poll(void);

// ----------------------------------------
// SCI_Dummy
//
// A testing stub. 
// Input:
//   none
// Output:
//   none
// Conditions:
//   Assumes transmit and receive FIFOs have been initialized
void SCI_Dummy(void);

#endif