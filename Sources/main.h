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
const UINT16 Timer_Ch0_Delay	=	1250;
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
UINT8 PWM_MAIN_LOOP = 0;

//Digital values for the DAC for both Voltage and Current
const TUINT16 DEM_VoltageTable3[] = 
{
	2047,
	2080,
	2112,
	2145,
	2177,
	2209,
	2240,
	2271,
	2302,
	2331,
	2361,
	2389,
	2417,
	2443,
	2469,
	2494,
	2517,
	2540,
	2561,
	2581,
	2600,
	2618,
	2634,
	2648,
	2662,
	2673,
	2684,
	2692,
	2699,
	2705,
	2709,
	2711,
	2712,
	2711,
	2709,
	2705,
	2699,
	2692,
	2684,
	2673,
	2662,
	2648,
	2634,
	2618,
	2600,
	2581,
	2561,
	2540,
	2517,
	2494,
	2469,
	2443,
	2417,
	2389,
	2361,
	2331,
	2302,
	2271,
	2240,
	2209,
	2177,
	2145,
	2112,
	2080,
	2047,
	2014,
	1982,
	1949,
	1917,
	1885,
	1854,
	1823,
	1792,
	1763,
	1733,
	1705,
	1677,
	1651,
	1625,
	1600,
	1577,
	1554,
	1533,
	1513,
	1494,
	1476,
	1460,
	1446,
	1432,
	1421,
	1410,
	1402,
	1395,
	1389,
	1385,
	1383,
	1382,
	1383,
	1385,
	1389,
	1395,
	1402,
	1410,
	1421,
	1432,
	1446,
	1460,
	1476,
	1494,
	1513,
	1533,
	1554,
	1577,
	1600,
	1625,
	1651,
	1677,
	1705,
	1733,
	1763,
	1792,
	1823,
	1854,
	1885,
	1917,
	1949,
	1982,
	2014,
	2047
};

const TUINT16 DEM_CurrentTable3[] = 
{
	2047,
	2057,
	2067,
	2077,
	2087,
	2097,
	2106,
	2116,
	2125,
	2135,
	2143,
	2152,
	2161,
	2169,
	2177,
	2184,
	2192,
	2199,
	2205,
	2211,
	2217,
	2223,
	2228,
	2232,
	2236,
	2240,
	2243,
	2246,
	2248,
	2249,
	2251,
	2251,
	2252,
	2251,
	2251,
	2249,
	2248,
	2246,
	2243,
	2240,
	2236,
	2232,
	2228,
	2223,
	2217,
	2211,
	2205,
	2199,
	2192,
	2184,
	2177,
	2169,
	2161,
	2152,
	2143,
	2135,
	2125,
	2116,
	2106,
	2097,
	2087,
	2077,
	2067,
	2057,
	2047,
	2037,
	2027,
	2017,
	2007,
	1997,
	1988,
	1978,
	1969,
	1959,
	1951,
	1942,
	1933,
	1925,
	1917,
	1910,
	1902,
	1895,
	1889,
	1883,
	1877,
	1871,
	1866,
	1862,
	1858,
	1854,
	1851,
	1848,
	1846,
	1845,
	1843,
	1843,
	1842,
	1843,
	1843,
	1845,
	1846,
	1848,
	1851,
	1854,
	1858,
	1862,
	1866,
	1871,
	1877,
	1883,
	1889,
	1895,
	1902,
	1910,
	1917,
	1925,
	1933,
	1942,
	1951,
	1959,
	1969,
	1978,
	1988,
	1997,
	2007,
	2017,
	2027,
	2037,
	2047
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

INT16 	DEM_Power_Array[POWER_ARRAY_SIZE];
INT16		DEM_Power_Squared_Array[POWER_ARRAY_SIZE];
TUINT16	DEM_Total_Energy;
TUINT16 DEM_Average_Power;
TUINT16 DEM_Total_Cost;

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

