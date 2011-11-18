// ----------------------------------------
// Filename: DEM.h
// Description: 
//  Digital energy meter implementation
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#include "DEM.h"

static TINT16 PowerArray[POWER_ARRAY_SIZE];
static TChannelNb voltage = Ch1;
static TChannelNb current = Ch2;


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
	for (i = 0; i < POWER_ARRAY_SIZE; i++)
	{
		PowerArray[i + 1] = PowerArray[i];
	}
	PowerArray[0] = Calculate_Power(voltage, current);
	
	
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