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

const INT16 DEM_VoltageTable[] = 
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

const INT16 DEM_CurrentTable[] = 
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

const INT16 DEM_VoltageTable1[] = 
{
	2047,
	2248,
	2446,
	2641,
	2830,
	3012,
	3184,
	3346,
	3494,
	3629,
	3749,
	3852,
	3938,
	4006,
	4055,
	4084,
	4094,
	4084,
	4055,
	4006,
	3938,
	3852,
	3749,
	3629,
	3494,
	3346,
	3184,
	3012,
	2830,
	2641,
	2446,
	2248,
	2047,
	1846,
	1648,
	1453,
	1264,
	1082,
	910,
	748,
	600,
	465,
	345,
	242,
	156,
	88,
	39,
	10,
	0,
	10,
	39,
	88,
	156,
	242,
	345,
	465,
	600,
	748,
	910,
	1082,
	1264,
	1453,
	1648,
	1846
};

const INT16 DEM_CurrentTable1[] = 
{
		2047,
	2248,
	2446,
	2641,
	2830,
	3012,
	3184,
	3346,
	3494,
	3629,
	3749,
	3852,
	3938,
	4006,
	4055,
	4084,
	4094,
	4084,
	4055,
	4006,
	3938,
	3852,
	3749,
	3629,
	3494,
	3346,
	3184,
	3012,
	2830,
	2641,
	2446,
	2248,
	2047,
	1846,
	1648,
	1453,
	1264,
	1082,
	910,
	748,
	600,
	465,
	345,
	242,
	156,
	88,
	39,
	10,
	0,
	10,
	39,
	88,
	156,
	242,
	345,
	465,
	600,
	748,
	910,
	1082,
	1264,
	1453,
	1648,
	1846
};

const INT16 DEM_VoltageTable2[] = 
{
	1023,
	1123,
	1223,
	1320,
	1414,
	1505,
	1591,
	1672,
	1746,
	1814,
	1874,
	1925,
	1968,
	2002,
	2026,
	2041,
	2046,
	2041,
	2026,
	2002,
	1968,
	1925,
	1874,
	1814,
	1746,
	1672,
	1591,
	1505,
	1414,
	1320,
	1223,
	1123,
	1023,
	923,
	823,
	726,
	632,
	541,
	455,
	374,
	300,
	232,
	172,
	121,
	78,
	44,
	20,
	5,
	0,
	5,
	20,
	44,
	78,
	121,
	172,
	232,
	300,
	374,
	455,
	541,
	632,
	726,
	823,
	923
};

const INT16 DEM_CurrentTable2[] = 
{
		1023,
	1123,
	1223,
	1320,
	1414,
	1505,
	1591,
	1672,
	1746,
	1814,
	1874,
	1925,
	1968,
	2002,
	2026,
	2041,
	2046,
	2041,
	2026,
	2002,
	1968,
	1925,
	1874,
	1814,
	1746,
	1672,
	1591,
	1505,
	1414,
	1320,
	1223,
	1123,
	1023,
	923,
	823,
	726,
	632,
	541,
	455,
	374,
	300,
	232,
	172,
	121,
	78,
	44,
	20,
	5,
	0,
	5,
	20,
	44,
	78,
	121,
	172,
	232,
	300,
	374,
	455,
	541,
	632,
	726,
	823,
	923
};

const TUINT16 DEM_VoltageTable3[] = 
{
	2047,
	2070,
	2093,
	2116,
	2139,
	2161,
	2184,
	2206,
	2227,
	2248,
	2269,
	2289,
	2309,
	2327,
	2346,
	2363,
	2380,
	2396,
	2411,
	2425,
	2438,
	2451,
	2462,
	2473,
	2482,
	2490,
	2498,
	2504,
	2509,
	2513,
	2516,
	2517,
	2518,
	2517,
	2516,
	2513,
	2509,
	2504,
	2498,
	2490,
	2482,
	2473,
	2462,
	2451,
	2438,
	2425,
	2411,
	2396,
	2380,
	2363,
	2346,
	2327,
	2309,
	2289,
	2269,
	2248,
	2227,
	2206,
	2184,
	2161,
	2139,
	2116,
	2093,
	2070,
	2047,
	2024,
	2001,
	1978,
	1955,
	1933,
	1910,
	1888,
	1867,
	1846,
	1825,
	1805,
	1785,
	1767,
	1748,
	1731,
	1714,
	1698,
	1683,
	1669,
	1656,
	1643,
	1632,
	1621,
	1612,
	1604,
	1596,
	1590,
	1585,
	1581,
	1578,
	1577,
	1576,
	1577,
	1578,
	1581,
	1585,
	1590,
	1596,
	1604,
	1612,
	1621,
	1632,
	1643,
	1656,
	1669,
	1683,
	1698,
	1714,
	1731,
	1748,
	1767,
	1785,
	1805,
	1825,
	1846,
	1867,
	1888,
	1910,
	1933,
	1955,
	1978,
	2001,
	2024,
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

/*
TUINT16 System_ElapsedSeconds;
TUINT16 System_ElapsedMinutes;
TUINT16 System_ElapsedHours;
TUINT16 System_ElapsedDays;
*/

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

