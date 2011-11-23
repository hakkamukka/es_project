// ----------------------------------------
// Filename: calc.c
// Description: 
//  Calculation implementation
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#include "calc.h"

static const INT8 N_VALUE = 16; // 2^4 for n

UINT16 Calculate_Power(const INT16 Voltage_Array[], const INT16 Current_Array[])
{
	INT16 voltageDigitalValue, currentDigitalValue;
	INT16 voltageValue, currentValue;
	INT16 powerValue;
	
	voltageDigitalValue = Voltage_Array[VOLTAGE_LOOP];
	currentDigitalValue = Current_Array[CURRENT_LOOP];
	
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

void Calcualte_TotalCost(void)
{
	/*DEM_Total_Cost += ( (DEM_Total_Energy.l >> BINARY_SHIFT_3) * (Clock_RunningTime() << 3) * sCurrentTariffRate );
	
	DEM_Total_Cost = DEM_Total_Cost >> 6;
	
	return DEM_Total_Cost;*/
	
	DEM_Total_Cost.l += ( ((DEM_Total_Energy.l * (sCurrentTariffRate)) + 1800 ) / 3600 );
	DEM_Total_Cost.l / 1000000000;
}

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

INT16 Calculate_DigitalToQNotation(const INT32 number)
{
	INT16 qValue;
	INT32 internalRepresentation;

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

/*
UINT16 Calculate_MultiplyQNotation(const TINT16 value1, const TINT16 value2, const UINT8 q)
{
	TINT16 result;
	//result = value1 * value2;
	return 0;
}*/

// ----------------------------------------
//Calcualte_Square_Root
//Description
//  Function takes the square rot of a number given a guess
//Input:
//  number is the number to take the square root of
//	guess is the initial guess of the square root
//Output:
//  the square root of the number
//Conditions:
//  GUESSING_ITERATIONS is enough to give an accurate number
UINT32 Calculate_Square_Root(const UINT32 number, const UINT32 guess)
{
	UINT8 i = 0;
	UINT32 result = 0;
	
	result = guess;
	
	for (i = 0; i < GUESSING_ITERATIONS; i++)
	{
		//Newton's method to find the square root
		result = ( ((number / result) + result) / 2 );
	}
	
	return result;
}

// ----------------------------------------
//Calculate_RMS
//Description
//  Calculates the RMS of a 16 value array consisting of INT16
//Input:
//  voltage - points to the beginning of the UINT16 voltage array
//	guess is the guess for the RMS
//Output:
//  calculated RMS
//Conditions:
//  the array contains 16 values
/*
UINT8 Calculate_RMS(const INT16 * const voltage, const UINT16 guess)
{
	UINT8 i = 0;
	UINT32 RMS = 0;
	
	for (i = 0; i < 16; i++)
	{
		RMS += ((INT32)voltage[i] * voltage[i]);
	}
	
	RMS /= 16;
	
	RMS = 
}
*/
/*
UINT16 Calculate_RMS(const INT16 powerArray[])
{
	INT16 i, temp, largest = 0;
	
	temp = powerArray[i];
	do
	{
		if (temp > powerArray[i + 1])
			largest = temp;
		else
			largest = powerArray[i + 1];
		i++;		
	}
	while (i < POWER_ARRAY_SIZE);
	
	return ( largest / Calculate_Square_Root(2, 2) );	
}
*/
void Calculate_RMS(const TChannelNb channel, const INT16 array[])
{
	INT8 i;
	INT16 square;
	INT16 sum;
	TUINT32 root;
	
	sum = 0;
	square = 0;
	root.l = 0;
	
	if (channel == Ch1)
	{
		for (i = 0; i < POWER_ARRAY_SIZE; i++)
		{
			square = array[i] * array[i];
			DEM_Voltage_Squared_Array[i] = square;
		}
		for (i = 0; i < POWER_ARRAY_SIZE; i++)
		{
			sum += sum + DEM_Voltage_Squared_Array[i];
		}
		root.l = Calculate_Square_Root(sum, 2);
		Voltage_RMS.l = root.s.Lo;
	}
	else if (channel == Ch2)
	{
		for (i = 0; i < POWER_ARRAY_SIZE; i++)
		{
			square = array[i] * array[i];
			DEM_Current_Squared_Array[i] = square;
		}
		for (i = 0; i < POWER_ARRAY_SIZE; i++)
		{
			sum += sum + DEM_Current_Squared_Array[i];
		}
		root.l = Calculate_Square_Root(sum, 2);
		Current_RMS.l = root.s.Lo;
	}
	
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

