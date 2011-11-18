// ----------------------------------------
// Filename: DEM.h
// Description: 
//  Digital energy meter implementation
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#ifndef DEF_H
#define DEF_H

#include <mc9s12a256.h>     /* derivative information */
#include "types.h"
#include "clock.h"
#include "EEPROM.h"
#include "calc.h"
#include "analog.h"

#define POWER_ARRAY_SIZE 16

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