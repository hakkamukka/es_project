// ----------------------------------------
// Filename: packet.c
// Description: Routines to implement packet
//   encoding and decoding for the serial port
// Author: Danny Sok
//         Edward Khou
// Date Created: 4-08-2011
// Date Last Modified: 10-08-2011

#include "packet.h"


static TState FSM[STATE_SIZE];
static INT8 StateIndex;
static BOOL bSwapFlag;


BOOL Packet_Setup(const UINT32 baudRate, const UINT32 busClk)
{
  (void)SCI_Setup(baudRate, busClk);
  StateIndex = 0;
  bSwapFlag = bFALSE;
   
  return bTRUE;
}

BOOL Packet_Put(const UINT8 command, const UINT8 parameter1, 
  const UINT8 parameter2, const UINT8 parameter3)
{
  if (!Timer_Enabled(TIMER_Ch7))
    Timer_Enable(TIMER_Ch7, bTRUE); // Will enable TDRF Interrupts
    
  return SCI_OutChar(command)     &&
         SCI_OutChar(parameter1)  &&
         SCI_OutChar(parameter2)  &&
         SCI_OutChar(parameter3)  &&
         SCI_OutChar( command ^ parameter1 ^ parameter2 ^ parameter3 );
}

BOOL Packet_Get(void)
{
  BOOL bStateInput = SCI_InChar(&FSM[StateIndex].data);  
 
  switch (StateIndex)
  {
    case 4:
      // If the FSM is full and the checksum matches, save it.
      if ( bStateInput && FSM[StateIndex].data == (FSM[0].data ^ FSM[1].data ^ FSM[2].data ^ FSM[3].data) )
      {
        Packet_Command    = FSM[0].data;
        Packet_Parameter1 = FSM[1].data;
        Packet_Parameter2 = FSM[2].data;
        Packet_Parameter3 = FSM[3].data;
        return bTRUE;
      }
      else if (bStateInput)  // If it isn't, and the state machine is full, then shuffle the bytes
        shuffleFSM();
      
      break;
  }
  
  if (bStateInput && !bSwapFlag) 
  {
    //FSM->next = *FSM[StateIndex++];
    StateIndex++;   // S0, -> 1, S1 -> 2 S4-> 5
  }
  
  return bFALSE; 
}

void shuffleFSM(void)
{
  INT8 i;
  for (i = 0; i < StateIndex; i++)
  {
    FSM[i].data = FSM[i+1].data;
    // 1 -> 0, 2->1, 3->2, 4->3  
  }
    
  // Clear a state for new input
  bSwapFlag = bTRUE; 
}