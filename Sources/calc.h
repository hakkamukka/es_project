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
#include "DEM.h"

#define VOLTAGE_DIGITAL_STEPS 1331
#define CURRENT_DIGITAL_STEPS 409
#define VOLTAGE_PEAK 					230
#define CURRENT_PEAK 					1

#define BINARY_SHIFT_3				3
#define BINARY_SHIFT_5				5
#define BINARY_SHIFT_10 			10

#define GUESSING_ITERATIONS 2

extern TAnalogInput Analog_Input[NB_INPUT_CHANNELS];

extern TUINT16 DEM_Total_Energy;

typedef enum
{
	qLeft,
	qRight
} TQNotationSide;

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
UINT16 Calculate_Power(TChannelNb voltage, TChannelNb current);

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
void Calculate_TotalEnergy(const INT16 array[]);

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

//UINT16 Calculate_TotalCost(void);

void Calcualte_TotalCost(void);

INT32 Calculate_NormalToQNotation(const INT32 value, const UINT8 shift);

INT16 Calculate_QNotationToNormal(const UINT32 value, const UINT8 shift);

INT16 Calculate_DigitalToQNotation(const INT32 number);

UINT16 Calculate_Division(UINT16 dividend, UINT16 divisor);



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