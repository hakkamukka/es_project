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
#include "HMI.h"
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

extern INT16		DEM_Voltage_Squared_Array[];
extern INT16		DEM_Current_Squared_Array[];

//extern TChannelNb ch1 = ch1;
//extern TChannelNb ch2;

//static TChannelNb voltage = Ch1;
//static TChannelNb current = Ch2;

//Loops
extern UINT8 VOLTAGE_LOOP;
extern UINT8 CURRENT_LOOP;

//RMS Calculations
extern TUINT16 Voltage_RMS;
extern TUINT16 Current_RMS;

typedef enum
{
	qLeft,
	qRight
} TQNotationSide;

// ----------------------------------------
//Calculate_Power
//Description
//	Calculates the power using voltage and current
//Input:
//  voltage and current are the respective analog channels  
//Output:
//  power value of unsigned long is returned
//Conditions:
//  
UINT16 Calculate_Power(const INT16 Voltage_Array[], const INT16 Current_Array[]);

// ----------------------------------------
//Calculate_TotalEnergy
//Description
//  Calculates the total energy by taking the sum of the
//	values of the arrays
//Input:
//  array[] is the array to be passed into for calculation
//Output:
//  none
//Conditions:
//  
void Calculate_TotalEnergy(const INT16 array[]);

// ----------------------------------------
//Calculate_TotalCost
//Description
//  Calculates the total cost using the energy
//Input:
//  none
//Output:
//  none
//Conditions:
//  Total energy determined
void Calcualte_TotalCost(void);

// ----------------------------------------
//Calculate_NormalToQNotation
//Description
//  Shifts the value by an amount
//Input:
//  value is the value to be shifted
//	shift is the amount to be shifted
//Output:
//  shifted value
//Conditions:
//  none
INT32 Calculate_NormalToQNotation(const INT32 value, const UINT8 shift);

// ----------------------------------------
//Calculate_QNotationToNormal
//Description
//  Unshifts
//Input:
//  
//Output:
//  
//Conditions:
//  
/*
INT16 Calculate_QNotationToNormal(const UINT32 value, const UINT8 shift);
*/

// ----------------------------------------
//Calculate_DigitalToQNotation
//Description
//  Converts the digital value from ADC to a real value
//Input:
//  Number is the number to be converted
//Output:
//  Scaled value
//Conditions:
//  none
INT16 Calculate_DigitalToQNotation(const INT32 number);

// ----------------------------------------
//Calculate_Division
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
// 
//UINT16 Calculate_Division(UINT16 dividend, UINT16 divisor);

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
//UINT16 Calculate_MultiplyQNotation(const TINT16 value1, const TINT16 value2, const UINT8 q);

// ----------------------------------------
//Calculate_Square_Root
//Description
//  Calculates the square root through the number of iterations
//Input:
//  The number to be rooted with an approximate guess
//Output:
//  Square root result
//Conditions:
//  none
UINT32 Calculate_Square_Root(const UINT32 number, const UINT32 guess);


// ----------------------------------------
//Calculate_RMS
//Description
//  Calculate the RMS of the value passed in
//Input:
//  
//Output:
//  
//Conditions:
//  
//UINT8 Calculate_RMS(const INT16 * const voltage, const UINT16 guess);
void Calculate_RMS(const TChannelNb channel, const INT16 array[]);





#endif