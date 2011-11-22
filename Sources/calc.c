// ----------------------------------------
// Filename: calc.c
// Description: 
//  Calculation implementation
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#include "calc.h"

static const INT8 N_VALUE = 16; // 2^4 for n

UINT16 Calculate_Power(TChannelNb voltage, TChannelNb current)
{
	INT16 voltageDigitalValue, currentDigitalValue;
	INT16 voltageValue, currentValue;
	INT16 powerValue;

	voltageDigitalValue = Analog_Input[voltage].Value.l - 2047;
	currentDigitalValue = Analog_Input[current].Value.l - 2047;
	
	//voltageValue.l = VOLTAGE_PEAK * (voltageDigitalValue.l / VOLTAGE_DIGITAL_STEPS);
	//currentValue.l = CURRENT_PEAK * (currentDigitalValue.l / CURRENT_DIGITAL_STEPS);
	
	voltageValue = ( Calculate_DigitalToQNotation(voltageDigitalValue) );
	currentValue = ( Calculate_DigitalToQNotation(currentDigitalValue) );
	
	powerValue = voltageValue * currentValue;
	return ( 1000 * (UINT16)powerValue );
}

void Calculate_TotalEnergy(const INT16 Power_Array[])
{
	INT8 i;
	for (i = 0; i < POWER_ARRAY_SIZE; i++)
	{
		DEM_Total_Energy.l += Power_Array[i];
	}
}

//UINT16 Calculate_TotalCost(void)
/*void Calcualte_TotalCost(void)
{
	DEM_Total_Cost += ( (DEM_Total_Energy.l >> BINARY_SHIFT_3) * (Clock_RunningTime() << 3) * sCurrentTariffRate );
	
	DEM_Total_Cost = DEM_Total_Cost >> 6;
	
	return DEM_Total_Cost;
}*/

INT32 Calculate_NormalToQNotation(const INT32 value, const UINT8 shift)
{
	INT32 qValue;
	qValue = value << shift;
	return qValue;
}

//needs fixing
/*
INT16 Calculate_QNotationToNormal(const UINT32 value, const TQNotationSide side, const UINT8 shift)
{
	INT32 normalValue;
	normalValue = value >> shift;
	
	switch(side)
	{
		case qLeft:
			return (normalValue / 1000);
			break;
		case qRight:
			return normalValue % 1000;
			break;
	}
	
}
*/

//INT16 Calculate_DigitalToQNotation(TChannelNb channel, const INT32 number)
INT16 Calculate_DigitalToQNotation(const INT32 number)
{
	INT16 qValue;
	INT32 internalRepresentation;
	
	/*
	switch(channel)
	{
		case Ch1:
			internalRepresentation = ( ((39 * number) + 8) / 16 );
			
			break;
		case Ch2:
			internalRepresentation = ( ((39 * number) + 8) / 16 );
			break;
	}
	*/

	internalRepresentation = ( ((39 * number) + 8) / 16 );
	
	qValue = Calculate_NormalToQNotation(internalRepresentation, 0);
	return qValue;

}

/*UINT16 Calculate_Division(UINT16 dividend, UINT16 divisor)
{
	UINT16 quotient = 0;
	UINT16 dividendAltered = dividend;

	do
	{
		dividendAltered -= divisor;
		quotient++;
	}
	while (dividendAltered > divisor);
	return quotient;

	if ( divisor > dividend )
	{
		return ~quotient;
	};
	
	
}*/


UINT16 Calculate_MultiplyQNotation(const TINT16 value1, const TINT16 value2, const UINT8 q)
{
	TINT16 result;
	//result = value1 * value2;
	return 0;
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
	UINT8 i = 0;
	UINT32 result = 0;
	
	result = guess;
	
	for (i = 0; i < GUESSING_ITERATIONS; i++)
	{
		result = ( ((number / result) + result) / 2 );
	}
	
	return result;
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
/*
UINT8 Calculate_RMS(const INT16 * const voltage, const UINT16 guess)
{
	UINT8 i = 0;
	UINT32 RMS = 0
	
	for (i = 0; i < 16; i++)
	{
		RMS += ((INT32)voltage[i] * voltage[i]);
	}
	
	RMS /= 16;
	
	RMS = 
}
*/
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

