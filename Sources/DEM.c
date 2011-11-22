// ----------------------------------------
// Filename: DEM.h
// Description: 
//  Digital energy meter implementation
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#include "DEM.h"

//#define PWM_SAMPLE_SIZE 16

static TChannelNb voltage = Ch1;
static TChannelNb current = Ch2;

TUINT16 totalPower, averagePower;

// ----------------------------------------
//DEM_Setup
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//
void DEM_Setup()
{
	DEM_Total_Energy.l 			= 0;
	DEM_Average_Power.l 		= 0;
	DEM_Total_Cost.l 				= 0;
}

void DEM_ArrayShift(INT16 array[])
{
	INT8 i;
	for (i = 0; i < POWER_ARRAY_SIZE - 1; i++)
	{
		array[i + 1] = array[i];
	}
}
// ----------------------------------------
//DEM_AveragePower
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//
void DEM_AveragePower()
{
	INT8 i;

	DEM_Power_Array[0] = Calculate_Power(voltage, current);
	
	for (i = 0; i < POWER_ARRAY_SIZE; i++)
	{
		totalPower.l = totalPower.l + DEM_Power_Array[i];
	}
	
	DEM_Average_Power.l = totalPower.l / POWER_ARRAY_SIZE;
}

void DEM_TotalCost()
{
	//sCurrentTariffRate
}

// ----------------------------------------
//DEM_CurrentTariff
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//
void DEM_CurrentTariff(void)
{
	if (sTariffNumber == 1)
	{
		if (System_Hours.s.Lo >= 14 && System_Hours.s.Lo < 20)
		{
			(void)EEPROM_Write16(&sCurrentTariffRate, sTariff1ToUPeak);
		}
		else if (System_Hours.s.Lo >= 7 && System_Hours.s.Lo < 14)
		{
			(void)EEPROM_Write16(&sCurrentTariffRate, sTariff1ToUShoulder);
		}
		else if (System_Hours.s.Lo >= 20 && System_Hours.s.Lo < 22)
		{
			(void)EEPROM_Write16(&sCurrentTariffRate, sTariff1ToUShoulder);
		}
		else
		{
			(void)EEPROM_Write16(&sCurrentTariffRate, sTariff1ToUOffPeak);
		}
	}
	else if (sTariffNumber == 2)
	{
		(void)EEPROM_Write16(&sCurrentTariffRate, sTariff2NonToU);
	}
	else if (sTariffNumber == 3)
	{
		(void)EEPROM_Write16(&sCurrentTariffRate, sTariff3NonToU);
	}
}