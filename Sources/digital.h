// ----------------------------------------
// Filename: digital.h
// Description: 
//  PWM and digital implementations
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#ifndef DIGITAL_H
#define DIGITAL_H

#include <mc9s12a256.h>     /* derivative information */
#include "types.h"
#include "SPI.h"

#define PWMWAVEFORMS 8
#define PWMDUTYPER_SIZE 2


typedef struct
{
	UINT8 currentRow, currentColumn;
	//UINT16 PWMDutyPer[PWMWAVEFORMS] [PWMDUTYPER_SIZE];
} TPWM;

// ----------------------------------------
//Digital_Setup
//Description: 
//Input:
//  
//Output:
//  
//Conditions:
//  
void Digital_Setup(void);

// ----------------------------------------
//Digital_Init
//Description: 
//Input:
//  
//Output:
//  
//Conditions:
//  
void Digital_Init(TPWM * const PWM);

// ----------------------------------------
//Digital_Put
//Description: 
//Input:
//  
//Output:
//  
//Conditions:
//  
void Digital_Put(void);

// ----------------------------------------
//
//Description: 
//Input:
//  
//Output:
//  
//Conditions:
//  


#endif