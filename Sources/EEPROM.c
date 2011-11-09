// ----------------------------------------
// Filename: EEPROM.c
// Description: Routines for erasing and
//   writing to the EEPROM
// Author: Danny Sok
//         Edward Khou
// Date Created: 24-08-2011
// Date Last Modified: 30-10-2011


#include "EEPROM.h"

 
 // EEPROM Commands
 const UINT8 EEPROM_SECTOR_ERASE   = 0x40;
 const UINT8 EEPROM_WORD_PROGRAM   = 0x20;
 const UINT8 EEPROM_MASS_ERASE     = 0x41;
 const UINT8 EEPROM_ERASE_VERIFY   = 0x05;

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

BOOL EEPROM_Setup(const UINT32 oscClk, const UINT32 busClk)
{
  if ((oscClk != 16000000) || (busClk != 24000000))
  {
    return bFALSE;
  }
  
  ECLKDIV_EDIV = 10;
  ECLKDIV_PRDIV8 = 1;
  
  if (sModConNb == 0xFFFF)
    (void)EEPROM_Write16(&sModConNb, 0x4F); //0x6003 = 24579, 0x4F = 79
    
  if (sModConMode == 0xFFFF)
    (void)EEPROM_Write16(&sModConMode, 1);
  
  if (Debug == 0xFFFF)
    (void)EEPROM_Write16(&Debug, 0);
  
  if (sControlMode == 0xFFFF)
    (void)EEPROM_Write16(&sControlMode, PACKET_ASYNCHRONOUS); 
 
  if (sNbAnalogInputs == 0xFFFF)
    (void)EEPROM_Write16(&sNbAnalogInputs, 2);
  
  if (sNbAnalogOutputs == 0xFFFF)
    (void)EEPROM_Write16(&sNbAnalogOutputs, 0);
  
  if (sAnalogPeriod == 0xFFFF)
    (void)EEPROM_Write16(&sAnalogPeriod, 10);
  
  if (sPacketPeriod == 0xFFFF)
    (void)EEPROM_Write16(&sPacketPeriod, 2);
  
  if (sLCDContrast == 0xFFFF)
    (void)EEPROM_Write16(&sLCDContrast, 50);
  
  return bTRUE;
}

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

BOOL EEPROM_Write32(UINT32 volatile * const address, const UINT32 data)
{                               
  TUINT32 address32, data32;
  address32.s.Lo = (UINT16)address;
  address32.s.Hi = address32.s.Lo + 2;
  
  data32.l = data;
  
  // If not in a 4 byte bound
  if (address32.s.Lo % 4 != 0) 
    return bFALSE; 
  
  return Write(EEPROM_SECTOR_ERASE, (volatile UINT16 *)address32.s.Lo, data32.s.Lo) &&
         Write(EEPROM_WORD_PROGRAM, (volatile UINT16 *)address32.s.Lo, data32.s.Lo) &&
         Write(EEPROM_WORD_PROGRAM, (volatile UINT16 *)address32.s.Hi, data32.s.Hi);
}

 
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

BOOL EEPROM_Write16(UINT16 volatile * const address, const UINT16 data)
{
  TUINT32 data32;
  UINT16 address16;
  address16 = (UINT16)address;
  
  // If not in a 2 byte bound
  if (address16 % 2 != 0) 
    return bFALSE; 
  
  // Check which word in sector
  // Align address to start of sector
  if (address16 % 4 == 0) 
  {
    data32.s.Lo = data;
    data32.s.Hi = _EW(address16+2);  
  } 
  else
  {
    // Save the data about to be overwritten
    data32.s.Lo = _EW(address16-2); 
    data32.s.Hi = data;
    // Decrease the address to align it to a 4 byte bound
    address16 -= 2;
  }
  
  (void)EEPROM_Write32((volatile UINT32 *)address16, data32.l);   
}

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

BOOL EEPROM_Write8(UINT8 volatile * const address, const UINT8 data)
{

  TUINT16 data16;
  UINT16 address16;
  
  address16 = (UINT16)address;
  // Check byte in word
  // Align address to start of word
  if (address16 % 2 == 0) 
  {
    data16.s.Hi = data;
    data16.s.Lo = _EB(address16 + 1);
  } 
  else
  {
    data16.s.Hi = _EB(address16 - 1);
    data16.s.Lo = data;
    // Decrease the address to align it to a 2 byte bound
    address16 --;
  }
  return EEPROM_Write16((volatile UINT16 *)address16, data16.l);
}


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

BOOL EEPROM_Erase(void)
{
  EPROT_EPDIS  = 1;          // EEPROM protection disabled
  EPROT_EPOPEN = 1;
  
  (void)Write(EEPROM_MASS_ERASE, (volatile UINT16 *)0x400, 0xFFFF);
  (void)Write(EEPROM_ERASE_VERIFY, (volatile UINT16 *)0x400, 0xFFFF);
    
  EPROT_EPDIS  = 0;          // EEPROM protection enabled
  EPROT_EPOPEN = 0;
  
  return ESTAT_BLANK; 
}

// ----------------------------------------
// EEPROM_Action
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

BOOL Write(UINT8 const command, UINT16 volatile * const address, const UINT16 data)
{
  ESTAT_PVIOL = 1;
  ESTAT_ACCERR = 1;
  
  if (ESTAT_CBEIF)
  {
    _EW(address) = data;
    ECMD = command;
    ESTAT_CBEIF = 1;
    
    if (ESTAT_ACCERR || ESTAT_PVIOL)
      return bFALSE;
    
    while (!ESTAT_CCIF)
    {
      ARMCOP = 0x55;
      ARMCOP = 0xAA;
    }
    
    return bTRUE;
  } 
  return bFALSE;    
}