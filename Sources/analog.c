                                                                                                                        // ----------------------------------------
// Filename: analog.c
// Description: Routines for setting up and
//   reading from the ADC/DAC
// Author: Danny Sok
//         Edward Khou
// Date: 24-09-2011
// Date Last Modified: 12-10-2011 

#include "analog.h"


static const INT16 ADC_OFFSET = 2048; // 12 Bit Offset, 1000 0000 0000

// ----------------------------------------
// Analog_Setup
// 
// Sets up the ADC and DAC
// Input:
//   none
// Output:
//   none
// Conditions:
//   none

void Analog_Setup(const UINT32 busClk) 
{
  
  TSPISetup aSPISetup;
    
  aSPISetup.isMaster          = bTRUE;                      // Master
  aSPISetup.activeLowClock    = bFALSE;                     // High clock
  aSPISetup.evenEdgeClock     = bTRUE;                      // Even
  aSPISetup.LSBFirst          = bFALSE;                     // MSB
  aSPISetup.baudRate          = 1000000;                    // 1 Mbit
  
  NbAnalogInputs.l            = 2;
  NbAnalogOutputs.l           = 2;
  
  SPI_Setup(&aSPISetup, busClk);
  
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
}

// ----------------------------------------
// Analog_Get
// 
// Gets an analog input channel's value based on the mode
// Input:
//   channelNb is the number of the anlog input channel to read
// Output:
//   none
// Conditions:
//   Assumes that the ADC has been set up

void Analog_Get(const TChannelNb channelNb)
{
  TUINT16 conversionResult;
  INT16 medianArray[3];
  INT16 x, y, temp;
  
  UINT8 txByte, txByte2, txByte3;
  txByte = 0x06; // Masking out D2 and don't cares [0000 0110]
  
  // 0000 00(d1)(d0)
  // (d1)(d0)00 0000
  txByte2 = channelNb << 6;   // Channel Select
  
  txByte3 = 0; // Don't cares
  
  // Value: 3 for Analog IO (Transmission, data to Rx)
  // Value: 0 for NULL since we don't send anything to the SPI.
  
  // 3 Low (Inverted) for transfer
  PTH_PTH4 = 1; 
  PTH_PTH5 = 1;
  PTH_PTH6 = 0;
  
  SPI_ExchangeChar(txByte,  &conversionResult.s.Hi);  // Rx Values are '?' so discard.
  SPI_ExchangeChar(txByte2, &conversionResult.s.Hi);  // Rx Values are NULL, B11->B8
  SPI_ExchangeChar(txByte3, &conversionResult.s.Lo);  // Rx Values are B7->B0
  conversionResult.s.Hi &= 0x0F;                      // Mask out the NULL bit [? ? ? 0 B11 B10 B09 B08]
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0; 
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  // Use median filter with sliding window of the 3 most recent samples
  Analog_Input[channelNb].Value3 = Analog_Input[channelNb].Value2;
  Analog_Input[channelNb].Value2 = Analog_Input[channelNb].Value1;
  Analog_Input[channelNb].Value1 = ADC_OFFSET - (INT16)conversionResult.l;
  
  Analog_Input[channelNb].OldValue.l = Analog_Input[channelNb].Value.l;
  
  medianArray[0] = Analog_Input[channelNb].Value1;
  medianArray[1] = Analog_Input[channelNb].Value2;
  medianArray[2] = Analog_Input[channelNb].Value3;
  
  // Bubble sort. Not the most efficient way, but there are only 3 variables to sift through
  for (x = 0; x < 3; x++)
  {
    for(y = 0; y < 2; y++)
    {
      if(medianArray[y] > medianArray[y+1])
      {// swap vars
        temp = medianArray[y];
        medianArray[y] = medianArray[y+1];
        medianArray[y+1] = temp;  
      } 
    }
  }
	
	Analog_Input[channelNb].Value.l = medianArray[1];

}

// ----------------------------------------
//Analog_Put
//Description
//  
//Input:
//  
//Output:
//  
//Conditions:
//  
void Analog_Put(const TChannelNb channelNb, TINT16 data)
{
	TUINT16 conversionResult;
  //INT16 medianArray[3];
  //INT16 x, y, temp;
  INT8 txByte, txByte2;
  
 	// 4 Low (Inverted) for transfer
  PTH_PTH4 = 0; 
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  if (channelNb == Ch1)
  {
  	txByte = 0x30;
  }
  else if (channelNb == Ch2)
  {
  	txByte = 0x70;
  }
  
  txByte |= (data.s.Hi & 0x0F);
  SPI_ExchangeChar(txByte, &conversionResult.s.Hi);
  txByte2 = data.s.Lo;
  SPI_ExchangeChar(txByte2, &conversionResult.s.Lo);

  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0; 
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  //Analog_Input[ChannelNb].Value.l = Analog_Input[channelNb].Value.l;
}