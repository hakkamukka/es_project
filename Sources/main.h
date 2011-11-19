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

#include "HMI.h"

#include "calc.h"

#include "DEM.h"

#include "digital.h"

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

const UINT8 CMD_TEST_MODE     = 0x10;
const UINT8 CMD_TARIFF        = 0x11;
const UINT8 CMD_TIME1         = 0x12;
const UINT8 CMD_TIME2         = 0x13;
const UINT8 CMD_POWER         = 0x14;
const UINT8 CMD_ENERGY        = 0x15;
const UINT8 CMD_COST          = 0x16;

const UINT8 CMD_FREQUENCY     = 0x17;
const UINT8 CMD_VOLTAGE_RMS   = 0x18;
const UINT8 CMD_CURRENT_RMS   = 0x19;
const UINT8 CMD_POWER_FACTOR  = 0x1A;

const UINT8 CMD_ACCELERATE    = 0x1B;

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
const UINT16 Periodic_Delay   = 20000; // 10ms
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

UINT8 PWM_MAIN_LOOP = 0;
//UINT16 DEM_VoltageTable[16];
//UINT16 DEM_CurrentTable[16];

const static INT16 DEM_VoltageTable[] = 
{
	0,
	201,
	399,
	594,
	783,
	965,
	1137,
	1299,
	1447,
	1582,
	1702,
	1805,
	1891,
	1959,
	2008,
	2037,
	2047,
	2037,
	2008,
	1959,
	1891,
	1805,
	1702,
	1582,
	1447,
	1299,
	1137,
	965,
	783,
	594,
	399,
	201,
	0,
	-201,
	-399,
	-594,
	-783,
	-965,
	-1137,
	-1299,
	-1447,
	-1582,
	-1702,
	-1805,
	-1891,
	-1959,
	-2008,
	-2037,
	-2047,
	-2037,
	-2008,
	-1959,
	-1891,
	-1805,
	-1702,
	-1582,
	-1447,
	-1299,
	-1137,
	-965,
	-783,
	-594,
	-399,
	-201
};

const static INT16 DEM_CurrentTable[] = 
{
	0,
	201,
	399,
	594,
	783,
	965,
	1137,
	1299,
	1447,
	1582,
	1702,
	1805,
	1891,
	1959,
	2008,
	2037,
	2047,
	2037,
	2008,
	1959,
	1891,
	1805,
	1702,
	1582,
	1447,
	1299,
	1137,
	965,
	783,
	594,
	399,
	201,
	0,
	-201,
	-399,
	-594,
	-783,
	-965,
	-1137,
	-1299,
	-1447,
	-1582,
	-1702,
	-1805,
	-1891,
	-1959,
	-2008,
	-2037,
	-2047,
	-2037,
	-2008,
	-1959,
	-1891,
	-1805,
	-1702,
	-1582,
	-1447,
	-1299,
	-1137,
	-965,
	-783,
	-594,
	-399,
	-201
};
/*
const static UINT16 DEM_VoltageTable[] = 
{
	1331, 1840, 2272, 2561, 2662, 2561, 2272, 1840, 1331, 822, 390, 101, 0, 101, 390, 822
};

const static UINT16 DEM_CurrentTable[] = 
{
	1155, 1597, 1972, 2222, 2310, 2222, 1972, 1597, 1155, 713, 338, 88, 0, 88, 338, 713
};
*/

UINT8 Clock_Seconds;
UINT8 Clock_Minutes;
UINT32 Clock_MicroSeconds;

TUINT16 System_Seconds;
TUINT16 System_Minutes;
TUINT16 System_Hours;
TUINT16 System_Days;

TUINT16 Total_Energy;
TUINT16 Average_Power;
TUINT16 Total_Cost;

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

//Project Basic Prototypes
BOOL HandleTestModePacket(void);
BOOL HandleTariffPacket(void);
BOOL HandleTime1Packet(void);
BOOL HandleTime2Packet(void);
BOOL HandlePowerPacket(void);
BOOL HandleEnergyPacket(void);
BOOL HandleCostPacket(void);

//Project Intermediate Prototypes
BOOL HandleFrequencyPacket(void);
BOOL HandleVoltageRMSPacket(void);
BOOL HandleCurrentRMSPacket(void);
BOOL HandlePowerFactorPacket(void);

//Project Misc Prototypes
BOOL HandleAcceleratePacket(void);

