// ----------------------------------------
// Filename: calc.c
// Description: 
//  Calculation implementation
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#include "calc.h"

static const INT8 N_VALUE = 16; // 2^4 for n

void Calculate_TariffQNotation(UINT16 const data)
{
  UINT16 number;
  UINT8 tenthousand, thousand, hundred, ten, single;
  tenthousand = 0;
  thousand    = 0;
  hundred     = 0;
  ten         = 0;
  single      = 0;
  number      = data;   
  
  if (number >= 10000)
  {
    tenthousand = (UINT8)(number / 10000);
    number -= (tenthousand * 10000);
    tenthousand |= 0x30;
  }
  if (number >= 1000) 
  {
    thousand = (UINT8)(number / 1000);
    number -= (thousand * 1000);
    thousand |= 0x30;
  }
  if (number >= 100)                       //if number is 300, 300/100 = 3
  {
    hundred = (UINT8)(number / 100);
    number -= (hundred * 100);             //wtfuggles?!?!?! must be a better way...
    hundred |= 0x30;                       //convert to ASCII by ORing with 0x30  
  }
  if (number >= 10)
  {
    ten = (UINT8)(number / 10);
    number -= (ten * 10);
    ten |= 0x30;  
  }
  if (number >= 0)
  {
    single = (UINT8)number;
    single |= 0x30; 
  }
  
  if (hundred == 0 && thousand != 0)
    hundred |= 0x30;
  
  if (ten == 0 && hundred != 0)
    ten |= 0x30;
  
  (void)LCD_OutChar(tenthousand);
  (void)LCD_OutChar(thousand);
  (void)LCD_OutChar('.');
  (void)LCD_OutChar(hundred);
  (void)LCD_OutChar(ten);
  (void)LCD_OutChar(single);
}