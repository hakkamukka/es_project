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
const UINT16 Periodic_Delay   = 20000; // 10ms //170.50
const UINT16 Timer_Ch7_Delay  = 2000;  // We need enough delay for 10 bits.
const UINT16 Timer_Ch0_Delay	=	10000;
const UINT16 Timer_Ch2_Delay 	= 10000;
const UINT16 Timer_Ch6_Delay  = 8000;
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

// Initialise Loop
UINT8 SAMPLE_VOLTAGE_LOOP = 0;
UINT8 SAMPLE_CURRENT_LOOP = 0;
UINT8 VOLTAGE_LOOP				= 0;
UINT8 CURRENT_LOOP				= 0;

//Display 
UINT8 Metering_Time_Display_Array[8];
UINT8 Cost_Display_Array[4];

//RMS Calculations
TUINT16 Voltage_RMS	;
TUINT16 Current_RMS	;


//Digital values for the DAC for both Voltage and Current
const TUINT16 DEM_VoltageTable3[] = 
{
	2047,
  2177,
  2302,
  2417,
  2517,
  2600,
  2662,
  2699,
  2712,
  2699,
  2662,
  2600,
  2517,
  2417,
  2302,
  2177,
  2047,
  1917,
  1792,
  1677,
  1577,
  1494,
  1432,
  1395,
  1382,
  1395,
  1432,
  1494,
  1577,
  1677,
  1792,
  1917

};

const TUINT16 DEM_CurrentTable3[] = 
{
	2047,
  2087,
  2125,
  2161,
  2192,
  2217,
  2236,
  2248,
  2252,
  2248,
  2236,
  2217,
  2192,
  2161,
  2125,
  2087,
  2047,
  2007,
  1969,
  1933,
  1902,
  1877,
  1858,
  1846,
  1842,
  1846,
  1858,
  1877,
  1902,
  1933,
  1969,
  2007
};

//Clock and time values
UINT8 Clock_Seconds;
UINT8 Clock_Minutes;
UINT32 Clock_MicroSeconds;

TUINT16 System_Seconds;
TUINT16 System_Minutes;
TUINT16 System_Hours;
TUINT16 System_Days;

/*
TUINT16 System_ElapsedSeconds;
TUINT16 System_ElapsedMinutes;
TUINT16 System_ElapsedHours;
TUINT16 System_ElapsedDays;
*/

INT16		DEM_Voltage_Array[POWER_ARRAY_SIZE];
INT16		DEM_Current_Array[POWER_ARRAY_SIZE];
INT16 	DEM_Power_Array[POWER_ARRAY_SIZE];
INT16		DEM_Power_Squared_Array[POWER_ARRAY_SIZE];
INT16		DEM_Voltage_Squared_Array[POWER_ARRAY_SIZE];
INT16		DEM_Current_Squared_Array[POWER_ARRAY_SIZE];
TUINT16	DEM_Total_Energy;
TUINT16 DEM_Average_Power;
TUINT32 DEM_Total_Cost;

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


