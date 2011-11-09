// ----------------------------------------
// Filename: FIFO.c
// Description: Routines to implement a FIFO buffer 
// Author: Danny Sok
//         Edward Khou
// Date Created: 4-08-2011
// Date Last Modified: 10-08-2011

#include "FIFO.h"

void FIFO_Init(TFIFO * const FIFO) 
{
  FIFO->Start   = 0;
  FIFO->End     = 0;  
  FIFO->NbBytes = 0;
}

BOOL FIFO_Put(TFIFO * const FIFO, const UINT8 data) 
{
  // Saves state of hardware
  EnterCritical();
  if (FIFO->NbBytes >= FIFO_SIZE)
  {
    //Error
    ExitCritical();
    return bFALSE;
  } 
  else
  {
    FIFO->Buffer[FIFO->End] = data;
    
    if(FIFO->End >= FIFO_SIZE - 1)      
      FIFO->End = 0;
    else
      FIFO->End++;
    
    FIFO->NbBytes++;
    ExitCritical();
    return bTRUE;
  }
}

BOOL FIFO_Get(TFIFO * const FIFO, UINT8 * const dataPtr)
{ 
  EnterCritical(); 
  if (FIFO->NbBytes == 0)
  {
    //Error
    ExitCritical();
    return bFALSE;
  } 
  else
  {
    *dataPtr = FIFO->Buffer[FIFO->Start];
    
    if(FIFO->Start >= FIFO_SIZE - 1)      
      FIFO->Start = 0;
    else
      FIFO->Start++;
  
    FIFO->NbBytes--;
    ExitCritical();
    return bTRUE;
  }
}

