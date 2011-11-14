// ----------------------------------------
// Filename: digital.h
// Description: 
//  PWM and digital implementations
// Author: Edward Khou
// Date Created: 14-11-2011
// Date Last Modified: 14-11-2011

#include "digital.h"

// ----------------------------------------
//Digital_PWMInit
//Description: 
//Input:
//  
//Output:
//  
//Conditions:
//  
void Digital_PWMInit(void)
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