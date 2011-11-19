// ----------------------------------------
// Filename: calc.c
// Description: 
//  Calculation implementation
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#include "calc.h"

static const INT8 N_VALUE = 16; // 2^4 for n

TUINT16 Calculate_Power(TChannelNb voltage, TChannelNb current)
{
	TINT16 voltageValue, currentValue;
	TINT32 powerValue;

	voltageValue.l = Analog_Input[voltage].Value.l;
	currentValue.l = Analog_Input[current].Value.l;
	
	//powerValue.l = voltageValue.l * currentValue.l;
}

UINT16 Calculate_TotalEnergy(void)
{
	
}

UINT16 Calculate_TotalCost(void)
{
	
}

UINT16 Calculate_MultiplyQNotation(const TINT16 value1, const TINT16 value2, const UINT8 q)
{
	
}

// ----------------------------------------
//Calculate_TariffQNotation
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//  
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

// ----------------------------------------
//Calcualte_Square_Root
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//  
UINT32 Calculate_Square_Root(const UINT32 number, const UINT32 guess)
{
	
}

// ----------------------------------------
//Calculate_RMS
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//  
UINT8 Calculate_RMS(const INT16 * const voltage, const UINT16 guess)
{
	
}

// ----------------------------------------
//Calculate_QNotation
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//

