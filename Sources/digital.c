// ----------------------------------------
// Filename: digital.h
// Description: 
//  PWM and digital implementations
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#include "digital.h"

static TPWM PWM;

// ----------------------------------------
//Digital_Setup
//Description: 
//Input:
//  
//Output:
//  
//Conditions:
//  
void Digital_Setup(void)
{
  PWME_PWME0 = 1;         //PWM channel 0 enabled
  PWME_PWME1 = 1;         //PWM channel 1 enabled
  
  PWMPOL_PPOL0 = 1;       //PWM channel 0 start high then go low when count reached
  PWMPOL_PPOL1 = 1;       //PWM channel 1 start high then go low when count reached 
  
  //PWMCLK
  
  //PWMPRCLK_PCKA = 
  //PWMPRCLK_PCKB = 
  
  PWMCAE_CAE0 = 0;        //PWM channel 0 left aligned output mode
  PWMCAE_CAE1 = 0;        //PWM channel 1 left aligned output mode
  
  Digital_Init(&PWM);
}

// ----------------------------------------
//Digital_Init
//Description: 
//Input:
//  
//Output:
//  
//Conditions:
//  
void Digital_Init(TPWM * const PWM)
{
	UINT16 PWM_ARRAY [PWMWAVEFORMS] [PWMDUTYPER_SIZE] =
	{
		{
			0, 256
		},
		{
			64, 256
		},
		{
			128, 256
		},
		{
			192, 256
		},
		{
			256, 256
		},
		{
			192, 256
		},
		{
			128, 256
		},
		{
			64, 256
		}
	};

	PWM->currentRow 		= 0;
	PWM->currentColumn	= 0;
}

// ----------------------------------------
//Digital_Put
//Description: 
//Input:
//  
//Output:
//  
//Conditions:
//  
void Digital_Put(void)
{
  
}