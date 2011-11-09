// ----------------------------------------
// Filename: packet.h
// Description: Routines to implement packet
//   encoding and decoding for the serial port
// Author: PMcL
// Date: 13-Mar-06

#ifndef PACKET_H
#define PACKET_H

// new types
#include "types.h"
#include "timer.h"
#include "SCI.h"

typedef struct
{
  UINT8 command;
  UINT8 parameter1;
  TUINT16 parameter23;
} TPacket;

#define Packet_Command     Packet.command
#define Packet_Parameter1  Packet.parameter1
#define Packet_Parameter2  Packet.parameter23.s.Lo
#define Packet_Parameter3  Packet.parameter23.s.Hi
#define Packet_Parameter23 Packet.parameter23.l
#define STATE_SIZE         5

typedef enum
{
  PACKET_ASYNCHRONOUS,
  PACKET_SYNCHRONOUS
} TPacketMode;

// Packet structure
extern TPacket Packet;
extern BOOL Packet_CommandOK;
extern TPacketMode Packet_Mode;

// Finate state machine structure
typedef struct
{
  UINT8 data;
  const struct TState *next;
} TState;

// ----------------------------------------
// Packet_Setup
// 
// Sets up the packets by calling the
// initialization routines of the supporting
// software modules
// Input:
//   baudRate is the baud rate in bits/sec
//   busClk is the bus clock rate in Hz
// Output:
//   TRUE if the packets were initialized successfully
// Conditions:
//   none

BOOL Packet_Setup(const UINT32 baudRate, const UINT32 busClk);

// ----------------------------------------
// Packet_Get
// 
// Attempts to get a packet from the receive buffer
// Input:
//   none
// Output:
//   TRUE if a valid packet was retrieved from the receive buffer
// Conditions:
//   none

BOOL Packet_Get(void);

// ----------------------------------------
// Packet_Put
// 
// Builds a packet and places it in the transmit buffer
// Input:
//   command is the packet command byte
//   parameter1 is the packet parameter1 byte
//   parameter2 is the packet parameter2 byte
//   parameter3 is the packet parameter3 byte
// Output:
//   TRUE if the packet was placed in the transmit buffer
// Conditions:
//   none

BOOL Packet_Put(const UINT8 command, const UINT8 parameter1, 
  const UINT8 parameter2, const UINT8 parameter3);
  
// ----------------------------------------
// shuffleFSM
// 
// Shuffles the finate state machine to the left by 1/
// Input:
//   none
// Output:
//   None
// Conditions:
//   Assumes the FSM is full
void shuffleFSM(void);

#endif