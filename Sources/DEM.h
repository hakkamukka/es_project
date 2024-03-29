// ----------------------------------------
// Filename: DEM.h
// Description: 
//  Digital energy meter implementation
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#ifndef DEM_H
#define DEM_H

#include <mc9s12a256.h>     /* derivative information */
#include "types.h"
#include "clock.h"
#include "EEPROM.h"
#include "calc.h"
#include "analog.h"

#define POWER_ARRAY_SIZE 16

extern	INT16			DEM_Voltage_Array[POWER_ARRAY_SIZE];
extern	INT16			DEM_Current_Array[POWER_ARRAY_SIZE];
extern 	INT16 		DEM_Power_Array[POWER_ARRAY_SIZE];
extern 	INT16			DEM_Power_Squared_Array[POWER_ARRAY_SIZE];
extern 	TUINT16		DEM_Total_Energy;
extern 	TUINT16 	DEM_Average_Power;

// ----------------------------------------
//DEM_Setup
//Description
//  Setup the DEM
//Input:
//  none
//Output:
//  none
//Conditions:
//	none
void DEM_Setup(void);

// ----------------------------------------
//DEM_ArrayShift
//Description
//  Shift an array
//Input:
//  array[] is the array to be shifted
//Output:
//  none
//Conditions:
//	none
void DEM_ArrayShift(INT16 array[]);

// ----------------------------------------
//DEM_TotalEnergy
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//
//void DEM_TotalEnergy(void);

// ----------------------------------------
//DEM_TotalCost
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//
//void DEM_TotalCost(void);

// ----------------------------------------
//DEM_AveragePower
//Description
//  Calculate the average power from DEM_Power_Array[16]
//Input:
//  none
//Output:
//  none
//Conditions:
//	none
void DEM_AveragePower(void);


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
void DEM_CurrentTariff(void);
#endif DEF_H