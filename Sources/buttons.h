// ----------------------------------------
// Filename: buttons.h
// Description: Lab 5
//  Implements the pushbutton HAL
// Author: Danny Sok
//         Edward Khou
// Date Created: 20-10-2011
// Date Last Modified: 22-10-2011

#ifndef BUTTONS_H
#define BUTTONS_H

#include <mc9s12a256.h>     /* derivative information */
#include "types.h"
#include "timer.h"

typedef enum
{
  
  Button0    = 0b00000100, // Bit2
  Button1    = 0b00001000, // Bit3
  Button2    = 0b00010000, // Bit4
  Button3    = 0b00100000, // Bit5
  Button4    = 0b10000000, // Bit7
  ButtonIdle = 0b00000000
} TButtonInputs;
  
// ----------------------------------------
// Buttons_Get
// 
// Gets the state of the pushbuttons
// Input:
//   
// Output:
//   none
// Conditions:
//  none
void Buttons_Get(TButtonInputs * const dataPtr); 



#endif