// ----------------------------------------
// Filename: main.h
// Description: Lab 5
//  Implements the ModCon serial protocol
//  Implements EEPROM write support and PLL
//  Implements interrupts and timers
// Author: Danny Sok
//         Edward Khou
// Date Created: 24-09-2011
// Date Last Modified: 20-10-2011

#include <hidef.h>          /* common defines and macros */
#include <mc9s12a256.h>     /* derivative information */
		 
#pragma LINK_INFO DERIVATIVE "mc9s12a256b"

// include files here
// new types
#include "types.h"

// Serial Communication Interface
#include "SCI.h"

// Packet handling
#include "packet.h"

// EEPROM
#include "EEPROM.h"

// Clocks and Reset Generator
#include "CRG.h"

// Timer - ETC
#include "timer.h"

// ADC
#include "analog.h"

// SPI Connection to ADC
#include "SPI.h"

// Version number
const UINT8 MAJOR_VERSION   = 1;
const UINT8 MINOR_VERSION   = 0;

// ----------------------------------------
// Packet_Commands

const UINT8 CMD_STARTUP       = 0x04;
const UINT8 CMD_EEPROM_PROG	  = 0x07;
const UINT8 CMD_EEPROM_GET	  = 0x08;
const UINT8 CMD_SPECIAL       = 0x09;
const UINT8 CMD_MODCON_NB     = 0x0b;
const UINT8 CMD_MODCON_MODE   = 0x0d;
const UINT8 CMD_MODCON_TIME   = 0x0c;
const UINT8 CMD_MODCON_PMODE  = 0x0a;
const UINT8 CMD_ANALOG_VALUE  = 0x50;

// ----------------------------------------
// Clock/rate setups
const UINT32 baudRate         = 115200;
const UINT32 busClk           = 24000000;  // 24Mhz
const UINT32 oscClk           = 16000000;  // 16Mhz
const UINT32 refClk           = 8000000;   // refClk is E_CLK normal? [busClk normal]
const UINT8  watchDogRate     = 1;         // 1ms stub for now
const static UINT8 NB_MSB     = 0x60;      // changed to 5 numbers
const static UINT8 NB_LSB     = 0x03;      //

// ----------------------------------------
// RTI const setups
const UINT8 prescaleRate      = 7;//0x07;
const UINT8 modulusCount      = 15;//0x0F;

// ----------------------------------------
// Timer Delays
const UINT16 Periodic_Delay   = 10000; // 10ms
const UINT16 Timer_Ch7_Delay  = 2000;  // We need enough delay for 10 bits.
/*
5339
5711
6682

Takes ~372 cycles per character in the packet
Takes ~1343 cycles per packet
So we should do like 1500-2000 for the delay in case?
*/

// ----------------------------------------
// Command setups
const static UINT8 copReset = 0x55;
const static UINT8 copEnd   = 0xAA;

// enumerated types here
// ----------------------------------------
// Packet setup
TPacket Packet;
BOOL Packet_CommandOK;
TPacketMode Packet_Mode;

// Analog Setup
TAnalogInput Analog_Input[NB_INPUT_CHANNELS];
TUINT16 NbAnalogInputs;
TUINT16 NbAnalogOutputs;

UINT8 Clock_Seconds;
UINT8 Clock_Minutes;
UINT32 Clock_MicroSeconds;

// ----------------------------------------
//  Function Prototypes
void main(void);
void HandlePacket(void);
BOOL HandleStartupPacket(void);
BOOL HandleSpecialPacket(void);
BOOL HandleNumberPacket(void);
BOOL HandleModePacket(void);
BOOL HandleEEPROMProgramPacket(void);
BOOL HandleEEPROMGetPacket(void);
BOOL HandleTimePacket(void);
BOOL HandlePacketModePacket(void);
BOOL HandleAnalogValPacket(TChannelNb channelNb);
