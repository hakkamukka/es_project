// ----------------------------------------
// Filename: SPI.c
// Description: I/O routines for MC9S12 serial
//   peripheral interface 
// Author: Danny Sok
//         Edward Khou
// Date Created: 24-09-2011
// Date Last Modified: 30-09-2011

#include "SPI.h"

// ----------------------------------------
// SPI_Setup
// 
// Sets up the Serial Peripheral Interface
// Input:
//   aSPISetup is a structure containing the parameters to 
//     be used in setting up the SPI:
//       isMaster is a Boolean value indicating whether the SPI is master or slave
//       activeLowClocks is a Boolean value indicating whether the clock is active
//         low or active high
//       evenEdgeClockPhase is a Boolean value indicating whether the data is clocked
//         on even or odd edges
//       LSBFirst is a Boolean value indicating whether the data is transferred LSB
//         first or MSB first
//       baudRate is the baud rate in bits/sec of the SPI clock
//   busClk is the bus clock rate in Hz
// Output:
//   none
// Conditions:
//   none
// ----------------------------------------

void SPI_Setup(const TSPISetup * const aSPISetup, const UINT32 busClk)
{
  // PORTH as per schematic
  UINT16 BRDivisor = (UINT16)(busClk / aSPISetup->baudRate);
  // XSPPRXSPR
  SPI0BR_SPR        = 2; // Rate Selection
  SPI0BR_SPPR       = 2; // Prescale
  
  SPI0CR1_SPIE      = 0;                                  // SPI Interrupt Enable, 0: Disabled
//  SPI0CR1_SPE       = 1;                                 // SPI System Enable, 1: Enabled
  SPI0CR1_SPTIE     = 0;                                  // SPI Transmit Interrupt, 0: Disable
  SPI0CR1_MSTR      = (UINT8)aSPISetup->isMaster;         // SPI Master/Slave. 0: Slave, 1: Master
  SPI0CR1_CPOL      = (UINT8)aSPISetup->activeLowClock;   // SPI Clock Polarity. 0: High, 1: Low
  SPI0CR1_CPHA      = (UINT8)aSPISetup->evenEdgeClock;    // SPI Clock Phase, 0: Odd, 1: Even
  SPI0CR1_SSOE      = 0;                                  // SPI Slave Output, refer to Pg.17
  SPI0CR1_LSBFE     = (UINT8)aSPISetup->LSBFirst;         // SPI LSB-First, 0: Most, 1: Least
  
  SPI0CR2_MODFEN    = 0;                                  // ModFault, 0: SS Port not used, 1: MODF Feature
  SPI0CR2_BIDIROE   = 0;                                  // Bidirectional Mode, 0: Disabled, 1: Enabled
  SPI0CR2_SPISWAI   = 0;                                  // Stop in Wait Mode, 0: Normal, 1: Stop clock
  SPI0CR2_SPC0      = 0;                                  // Serial Pin Control, Page 18.
    
  SPI0CR1_SPE       = 1;                                  // SPI System Enable, 1: Enabled
  
  // Init Port H Directions to Output
  DDRH_DDRH4        = 1;
  DDRH_DDRH5        = 1;
  DDRH_DDRH6        = 1; 
}
 
// ----------------------------------------
// SPI_ExchangeChar
//
// Transmits a byte and retrieves a received byte from the SPI
// Input:
//   dataTx is a byte to transmit
//   dataRx is a pointer to a byte to receive
// Output:
//   none
// Conditions:
//   Assumes SPI has been set up

void SPI_ExchangeChar(const UINT8 dataTx, UINT8 * const dataRx)
{ 
  while (!SPI0SR_SPTEF);
  
    // Wait for TX
  SPI0DR = dataTx;
  
  while (!SPI0SR_SPIF);
  
    // Wait for RX
  *dataRx = SPI0DR;
      
}