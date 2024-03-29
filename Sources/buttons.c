// ----------------------------------------
// Filename: buttons.c
// Description: Lab 5
//  Implements the pushbutton HAL
// Author: Danny Sok
//         Edward Khou
// Date Created: 20-10-2011
// Date Last Modified: 2-11-2011

#include "buttons.h"


// ----------------------------------------
// Buttons_Get
// 
// Gets the state of the pushbuttons
// Input:
//   dataPtr is a pointer to the button
// Output:
//   none
// Conditions:
//  none
void Buttons_Get(TButtonInputs * const dataPtr) 
{
  //Timer_Set(TIMER_Ch5, bFALSE);
  *dataPtr = PORTK & 0xBC;
  
  //Wait Until the button is released
  while ( (TFLG1_C5F) );
  

  
  // 10111100 
  // 1X1111XX
  // MSB: BC, LSB: 3D
  
}

