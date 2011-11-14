// ----------------------------------------
// Filename: EEPROM.h
// Description: Routines for erasing and
//   writing to the EEPROM
// Author: PMcL
// Date: 16-Mar-06

#ifndef EEPROM_H
#define EEPROM_H

// new types
#include "types.h"
#include "packet.h"

#include <mc9s12a256.h>     /* derivative information */

// EEPROM data access
#define _EB(EEPROM_ADDRESS)	  *(UINT8 volatile *)(EEPROM_ADDRESS)
#define _EI(EEPROM_ADDRESS)	  *(INT16 volatile *)(EEPROM_ADDRESS)
#define _EW(EEPROM_ADDRESS)	  *(UINT16 volatile *)(EEPROM_ADDRESS)
#define _EL(EEPROM_ADDRESS)	  *(INT32 volatile *)(EEPROM_ADDRESS)
#define _ES(EEPROM_ADDRESS)	  *(UINT32 volatile *)(EEPROM_ADDRESS)

// ----------------------------------------
// EEPROM addresses
// ----------------------------------------

// ModCon parameters
#define     sModConNb         _EW(0x400)
#define     sModConMode       _EW(0x402)
#define     sControlMode      _EW(0x404)          
#define     sNbAnalogInputs   _EW(0x406)
#define     sNbAnalogOutputs  _EW(0x408)

// Time periods
#define     sAnalogPeriod     _EW(0x410)
#define     sPacketPeriod     _EW(0x412)

// Debug flag
#define     Debug             _EW(0x420)

// EEPROM erased flag
#define     EEPROMErasedFlag  _EW(0x430)

// LCD settings
#define     sLCDContrast      _EW(0x440)

//Tariff settings
#define     sTariffNumber         _EW(0x450)
#define     sTariff1ToUPeak       _EW(0x460)
#define     sTariff1ToUShoulder   _EW(0x470)
#define     sTariff1ToUOffPeak    _EW(0x480)

#define     sTariff2NonToU        _EW(0x490)

#define     sTariff3NonToU        _EW(0x500)

/*
#define     sTariff2Peak          _EL(0x450)
#define     sTariff2Shoulder      _EL(0x460)
#define     sTariff2OffPeak       _EL(0x470)

#define     sTariff3Peak          _EL(0x450)
#define     sTariff3Shoulder      _EL(0x460)
#define     sTariff3OffPeak       _EL(0x470)
*/



                            
// ----------------------------------------
// EEPROM_Setup
// 
// Sets up the EEPROM with the correct internal clock
// Based on Figure 4-1 of the EETS4K Block User Guide V02.07
// Input:
//  oscClk is the oscillator clock frequency in Hz
//  busClk is the bus clock frequency in Hz
// Output:
//   TRUE if the EEPROM was setup succesfully
// Conditions:
//   none

BOOL EEPROM_Setup(const UINT32 oscClk, const UINT32 busClk);
 
// ----------------------------------------
// EEPROM_Write32
// 
// Writes a 32-bit number to EEPROM
// Input:
//   address is the address of the data,
//   data is the data to write
// Output:
//   TRUE if EEPROM was written successfully
//   FALSE if address is not aligned to a 4-byte boundary
//   or if there is a programming error
// Conditions:
//   Assumes EEPROM has been initialized

BOOL EEPROM_Write32(UINT32 volatile * const address, const UINT32 data);
 
// ----------------------------------------
// EEPROM_Write16
// 
// Writes a 16-bit number to EEPROM
// Input:
//   address is the address of the data,
//   data is the data to write
// Output:
//   TRUE if EEPROM was written successfully
//   FALSE if address is not aligned to a 2-byte boundary
//   or if there is a programming error
// Conditions:
//   Assumes EEPROM has been initialized

BOOL EEPROM_Write16(UINT16 volatile * const address, const UINT16 data);

// ----------------------------------------
// EEPROM_Write8
// 
// Writes an 8-bit number to EEPROM
// Input:
//   address is the address of the data,
//   data is the data to write
// Output:
//   TRUE if EEPROM was written successfully
//   FALSE if there is a programming error
// Conditions:
//   Assumes EEPROM has been initialized

BOOL EEPROM_Write8(UINT8 volatile * const address, const UINT8 data);

// ----------------------------------------
// EEPROM_Erase
// 
// Erases the entire EEPROM
// Input:
//   none
// Output:
//   TRUE if EEPROM was erased successfully
// Conditions:
//   Assumes EEPROM has been initialized

BOOL EEPROM_Erase(void);

// ----------------------------------------
// Write
// 
// Performs commands: program, sector erase, mass erase and erase verify
// Input:
//   command is the EEPROM command
//   address is the address of the data,
//   data is the data to write
// Output:
//   TRUE if EEPROM command was successful
// Conditions:
//   none

BOOL Write(UINT8 const command, UINT16 volatile * const address, const UINT16 data);



#endif