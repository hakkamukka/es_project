// ----------------------------------------
// Filename: calc.h
// Description: 
//  Calculation implementation
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#ifndef CALC_H
#define CALC_H

#include <mc9s12a256.h>     /* derivative information */
#include "types.h"
#include "LCD.h"
#include "analog.h"

#define VOLTAGE_DIGITAL_STEPS 942
#define CURRENT_DIGITAL_STEPS 409
#define VOLTAGE_PEAK 230
#define CURRENT_PEAK 1

extern TAnalogInput Analog_Input[NB_INPUT_CHANNELS];

// ----------------------------------------
//Calculate_AveragePower
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//  
TUINT16 Calculate_Power(TChannelNb voltage, TChannelNb current);

// ----------------------------------------
//Calculate_TotalEnergy
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//  
UINT16 Calculate_TotalEnergy(void);

// ----------------------------------------
//Calculate_TotalCost
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//  
UINT16 Calculate_TotalCost(void);

UINT16 Calculate_QNotation(UINT16 value, INT8 base);

UINT16 Calculate_Division(UINT16 dividend, UINT16 divisor);

UINT16 Calculate_EquivalentValue(const TINT16 voltage);

// ----------------------------------------
//Calculate_MultiplyQNotation
//Description	
//  
//Input:
//  
//Output:
//  
//Conditions:
//  
UINT16 Calculate_MultiplyQNotation(const TINT16 value1, const TINT16 value2, const UINT8 q);

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
void Calculate_TariffQNotation(const UINT16 value);

// ----------------------------------------
//Calculate_Square_Root
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//  
UINT32 Calculate_Square_Root(const UINT32 number, const UINT32 guess);

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
UINT8 Calculate_RMS(const INT16 * const voltage, const UINT16 guess);





#endif