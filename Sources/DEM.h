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

extern TUINT16 Total_Energy;
extern TUINT16 Average_Power;
extern TUINT16 Total_Cost;

//extern UINT16 DEM_VoltageTable[16];
//extern UINT16 DEM_CurrentTable[16];
extern INT16 DEM_VoltageTable[];
extern INT16 DEM_CurrentTable[];

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
void DEM_Setup(void);

// ----------------------------------------
//DEM_Energy
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//
void DEM_Energy(void);

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