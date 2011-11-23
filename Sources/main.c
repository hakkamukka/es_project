// ----------------------------------------
// Filename: main.c
// Description: Lab 5
//  Implements the ModCon serial protocol
//  Implements EEPROM write support and PLL
//  Implements interrupts and timers
// Author: Danny Sok
//         Edward Khou
// Date Created: 24-09-2011
// Date Last Modified: 20-10-2011

// Constants and dependencies
#include "main.h"

// ----------------------------------------
// main
//
// Initializes the various peripherals
// Handles received packets endlessly
// Input: none
// Output: none
// Conditions: none
//

void main(void)
{
  // Setup Routines.
  BOOL bSetupState;
  TTimerSetup timerSetup;
  TTimerSetup timerSetupCh0;
  
  
  bSetupState = Packet_Setup(baudRate, busClk) &&
                EEPROM_Setup(oscClk, busClk) &&
                CRG_SetupPLL(busClk, oscClk, refClk) &&
                CRG_SetupCOP(watchDogRate);
                
                

  timerSetup.outputCompare = bTRUE;
  timerSetup.outputAction = TIMER_OUTPUT_DISCONNECT;      
  timerSetup.inputDetection = TIMER_INPUT_OFF;
  timerSetup.toggleOnOverflow = bFALSE;
  timerSetup.interruptEnable = bTRUE;
  
  timerSetupCh0.outputCompare = bTRUE;
  timerSetupCh0.outputAction = TIMER_OUTPUT_DISCONNECT;
  timerSetupCh0.inputDetection = TIMER_INPUT_OFF;
  timerSetupCh0.toggleOnOverflow = bFALSE;
  timerSetupCh0.interruptEnable = bTRUE;
                
  Clock_Setup(prescaleRate, modulusCount);
  Analog_Setup(busClk);
  
  Timer_Setup();
  Timer_PeriodicTimerEnable(bTRUE);
  Timer_SetupPeriodicTimer(Periodic_Delay, busClk);
  
  //Timer_SetupPulseAccumulators();
  Timer_Init(TIMER_Ch7, &timerSetup);
  Timer_Set(TIMER_Ch7, Timer_Ch7_Delay);
  
  //Setup ECT_Ch0
  Timer_Init(TIMER_Ch0, &timerSetupCh0);
  Timer_Set(TIMER_Ch0, Timer_Ch0_Delay);
  Timer_Enable(TIMER_Ch0, bTRUE); //enables the timer channel 0 interrupt
  
  //Setup ECT_Ch6
  Timer_Init(TIMER_Ch2, &timerSetupCh0);
  Timer_Set(TIMER_Ch2, Timer_Ch2_Delay);
  Timer_Enable(TIMER_Ch2, bFALSE); //enables the timer channel 0 interrupt
  
  HMI_Setup();
  
  DEM_Setup();
  
  if (bSetupState)
  {
    // Turn on LED D11 to show success.
    DDRE_BIT7   = 1;
    PORTE_BIT7  = 0;
  }
  
   //SCI_Dummy();
  // Send packets on startup
  
  EnableInterrupts;
  (void)HandleStartupPacket();

  // Main loop
  for (;;)
  {
    ARMCOP = copReset;
    //SCI_Poll();
    
    if ( Packet_Get() )
      HandlePacket();
    
    // Determine if a second has passed. If it has then send a time packet  
    if (Clock_Update())
    {
      // && (Debug == 1) )
      //(void)HandleTimePacket();
      (void)DEM_CurrentTariff();
      (void)DEM_ArrayShift(DEM_Power_Array);
      (void)DEM_AveragePower();
      (void)Calculate_TotalEnergy(DEM_Power_Array);
      (void)Calcualte_TotalCost();
      (void)Calculate_RMS(Ch1, DEM_Power_Array);
      (void)Calculate_RMS(Ch2, DEM_Current_Array);
      (void)HMI_Update();
    }
    ARMCOP = copEnd;
  }
}

//---------------------------------
//  HandlePacket
//
//  Determines what packet is recieved, then replies with the
//  coresponding answer
//  Input: none
//  Output: none
//  Conditions: none
void HandlePacket(void)
{
  //UINT8 unknownPacket = Packet_Command | 0x80; // Determine if ACK was requested
  BOOL bACK = bFALSE;

  if (Packet_Command >= 0x80) // 128 in hex
  {
    bACK = bTRUE;
    Packet_Command -= 0x80;  
  }
  
  switch (Packet_Command)
  {
    case CMD_STARTUP:
         Packet_CommandOK = HandleStartupPacket();
         break;
    case CMD_SPECIAL:
         Packet_CommandOK = HandleSpecialPacket();
         break;
    case CMD_MODCON_NB:
         Packet_CommandOK = HandleNumberPacket();
         break;
    case CMD_MODCON_MODE:
         Packet_CommandOK = HandleModePacket();
         break;
    case CMD_EEPROM_PROG:
         Packet_CommandOK = HandleEEPROMProgramPacket();
         break;
    case CMD_EEPROM_GET:
         Packet_CommandOK = HandleEEPROMGetPacket();
         break;
    case CMD_MODCON_PMODE:
         Packet_CommandOK = HandlePacketModePacket();
         break;
    //case CMD_ANALOG_VALUE:
         //Packet_CommandOK = HandleAnalogValPacket();
         //break;
    case CMD_TEST_MODE:
         Packet_CommandOK = HandleTestModePacket();
         break;
    case CMD_TARIFF:
         Packet_CommandOK = HandleTariffPacket();
         break;
    case CMD_TIME1:
         Packet_CommandOK = HandleTime1Packet();
         break;
    case CMD_TIME2:
         Packet_CommandOK = HandleTime2Packet();
         break;
    case CMD_POWER:
         Packet_CommandOK = HandlePowerPacket();
         break;
    case CMD_ENERGY:
         Packet_CommandOK = HandleEnergyPacket();
         break;
    case CMD_COST:
         Packet_CommandOK = HandleCostPacket();
         break;
    case CMD_FREQUENCY:
         Packet_CommandOK = HandleFrequencyPacket();
         break;
    case CMD_VOLTAGE_RMS:
         Packet_CommandOK = HandleVoltageRMSPacket();
         break;
    case CMD_CURRENT_RMS:
         Packet_CommandOK = HandleCurrentRMSPacket();
         break;
    case CMD_POWER_FACTOR:
         Packet_CommandOK = HandlePowerFactorPacket();
         break;     
    default:
         //(void)HandleErrorPacket();
         Packet_CommandOK = bFALSE;
         break;
  }
         
  if (bACK && Packet_CommandOK)
  {
    (void)Packet_Put(Packet_Command += 0x80, Packet_Parameter1, Packet_Parameter2, Packet_Parameter3);
  }
  else if (bACK)
    (void)Packet_Put(Packet_Command, Packet_Parameter1, Packet_Parameter2, Packet_Parameter3);
  

  
 }

//---------------------------------
//  MCCNT_ISR
//
//  The ISR for Modulus Count.
//  This will send the analog values to the pc
//  Input: none
//  Output: none
//  Conditions: none    
void interrupt 26 MCCNT_ISR(void)
{
	MCFLG_MCZF = 1; // Clear/Ack

	if (Debug)
	{
		PTT_PTT4 ^= 1;
		
		if (SAMPLE_CURRENT_LOOP == 31)
			SAMPLE_CURRENT_LOOP = 0;

		Analog_Put(Ch2, DEM_CurrentTable3[SAMPLE_CURRENT_LOOP]);
		SAMPLE_CURRENT_LOOP++;

		Analog_Get(Ch2);
		
		if (CURRENT_LOOP < 16)
		{
			DEM_Current_Array[CURRENT_LOOP] = Analog_Input[Ch2].Value.l;
			CURRENT_LOOP++;
		}
		else
		{
			CURRENT_LOOP = 0;
			DEM_Current_Array[CURRENT_LOOP] = Analog_Input[Ch2].Value.l;
		}
		
		
	}
	// In ASYNC mode, we send only if the value has changed.
	if (sControlMode == PACKET_ASYNCHRONOUS)
	{
	  if (Analog_Input[Ch1].OldValue.l != Analog_Input[Ch1].Value.l)
	    (void)HandleAnalogValPacket(Ch1);
	  if (Analog_Input[Ch2].OldValue.l != Analog_Input[Ch2].Value.l)
	    (void)HandleAnalogValPacket(Ch2);
	}
	// In SYNC mode, we send all the time.
	else
	{
	  (void)HandleAnalogValPacket(Ch1);
	  (void)HandleAnalogValPacket(Ch2);
	}


}

//---------------------------------
//  ECTCh0_ISR
//
//  The ISR for ECT Channel 0
//  This will send and receive the voltaeg values.
//  Input: none
//  Output: none
//  Conditions: none    
void interrupt 8 ECTCh0_ISR(void)
{
	//UINT8 SCI;
	TFLG1_C0F = 1; 		//ACK the interrupt on ECT_Ch0
	//SCI = SCI0SR1;
	//Timer_Set(TIMER_Ch0, Timer_Ch0_Delay);
	
	if(Debug)
	{
		if (SAMPLE_VOLTAGE_LOOP == 31)
			SAMPLE_VOLTAGE_LOOP = 0;
	
		Analog_Put(Ch1, DEM_VoltageTable3[SAMPLE_VOLTAGE_LOOP]);
		SAMPLE_VOLTAGE_LOOP++;

		Analog_Get(Ch1);
		
		if (VOLTAGE_LOOP < 16)
		{
			DEM_Voltage_Array[VOLTAGE_LOOP] = Analog_Input[Ch1].Value.l;
			VOLTAGE_LOOP++;
		}
		else
		{
			VOLTAGE_LOOP = 0;
			DEM_Voltage_Array[VOLTAGE_LOOP] = Analog_Input[Ch1].Value.l;
		}
	}



}

//---------------------------------
//  ECTCh2_ISR
//
//  The ISR for ECT Channel 2
//  This will send and receive the current values
//  Input: none
//  Output: none
//  Conditions: none    
void interrupt 10 ECTCh2_ISR(void)
{
  TFLG1_C2F = 1; 
  /*if (SAMPLE_CURRENT_LOOP == 31)
		SAMPLE_CURRENT_LOOP = 0;

	Analog_Put(Ch2, DEM_CurrentTable3[SAMPLE_CURRENT_LOOP]);
	SAMPLE_CURRENT_LOOP++;

	Analog_Get(Ch2);*/
}

//---------------------------------
//  HandleStartupPacket
//
//  Creates the packet that will be in
//  response to the startup call by the PC.
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleStartupPacket(void)
{
  if(Packet_Parameter1 != 0 || Packet_Parameter1 != 0 || Packet_Parameter3 != 0)
  {
    return bFALSE;  
  }
    
  
  return Packet_Put(CMD_STARTUP, 0, 0, 0) && // startup takes 0 for all parameters
         HandleSpecialPacket() &&
         HandleNumberPacket() &&
         HandleModePacket() &&
         Packet_Put( CMD_MODCON_PMODE, 1, (UINT8)sControlMode, 0 );
}

//---------------------------------
//  HandleSpecialPacket
//
//  Creates the packet that will be in
//  response to the special call by the PC.
//  It responds with the ModCon Version.
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleSpecialPacket(void)
{
  switch (Packet_Parameter1)
  {
    case 'v':
      return Packet_Put(CMD_SPECIAL, 'v', MAJOR_VERSION, MINOR_VERSION);
      break;
    case 'd':
      EEPROM_Write16(&Debug, 1); // Set debug
      return HandleTimePacket();
      break;
  }
  
  // If called by Startup Packet
  return Packet_Put(CMD_SPECIAL, 'v', MAJOR_VERSION, MINOR_VERSION);
}

//---------------------------------
//  HandleNumberPacket
//
//  Creates the packet that will be in
//  response to the ModCon number call by the PC.
//  It responds with the last five digits of our
//  student number.
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleNumberPacket(void)
{
  TUINT16 NbModCon;
  if (Packet_Parameter1 == 2)
  {
    NbModCon.s.Lo = Packet_Parameter2;
    NbModCon.s.Hi = Packet_Parameter3;
    
  }
  else
  {
    NbModCon.l = sModConNb;
  }
  
  return Packet_Put(CMD_MODCON_NB, 1, NbModCon.s.Lo, NbModCon.s.Hi);
  
}

//---------------------------------
//  HandleModePacket
//
//  Creates the packet that will be in
//  response to the ModCon mode call by the PC.
//  It responds with the modcon mode
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleModePacket(void)
{
  TUINT16 mode;
  mode.l = sModConMode;
  return Packet_Put(CMD_MODCON_MODE, 1, mode.s.Lo, mode.s.Hi);
}

//---------------------------------
//  HandleEEPROMProgramPacket
//
//  Creates the packet that will be in
//  response to the ModCon EEPROM Programming call by the PC.
//  It responds by writing the data recieved
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleEEPROMProgramPacket(void)
{
  TUINT16 address;
  address.s.Lo = Packet_Parameter1;    //LSB
  address.s.Hi = Packet_Parameter2;    //MSB
  
  if (address.s.Hi == 10 && address.s.Lo == 0)
    return EEPROM_Erase();

  return EEPROM_Write8( (volatile UINT8 *)address.l, Packet_Parameter3 ); 
}

//---------------------------------
//  HandleEEPROMGetPacket
//
//  Creates the packet that will be in
//  response to the ModCon mode call by the PC.
//  It responds with the modcon mode
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleEEPROMGetPacket(void)
{
  TUINT16 address;
  address.s.Lo = Packet_Parameter1;
  address.s.Hi = Packet_Parameter2;
  
  if ( (Packet_Parameter2 > 0x0F) || (Packet_Parameter2 < 0x04) )
    return bFALSE;
  
  return Packet_Put( CMD_EEPROM_GET, Packet_Parameter1, Packet_Parameter2, _EB(address.l) ); // cmd, lsb, msb, data
}

//---------------------------------
//  HandleTimePacket
//
//  Creates the packet that will be in
//  response to the ModCon time call by the PC.
//  It responds with the modcon time
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleTimePacket(void)
{
  // Intervals : 'i', Seconds, Minutes
  return Packet_Put(CMD_MODCON_TIME, 'i', Clock_Seconds, Clock_Minutes);
}

//---------------------------------
//  HandlePacketModePacket
//
//  Creates the packet that will be in
//  response to the ModCon Packet Mode call by the PC.
//  It responds with the Packet Mode
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandlePacketModePacket(void)
{
  // p1: 2 = set, 1 = get
  // p2: 0 = async, 1 = sync
  // p3: 0
  if (Packet_Parameter1 == 2)
    return EEPROM_Write16(&sControlMode, Packet_Parameter2);
  if (Packet_Parameter1 == 1)
    return Packet_Put( CMD_MODCON_PMODE, 1, (UINT8)sControlMode, 0 ); // cmd, 1, mode, 0;  
}

//---------------------------------
//  HandleAnalogValPacket
//
//  Creates the packet that will be in
//  response to the ModCon Analog Value call by the PC.
//  It responds with the current Analog Value
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleAnalogValPacket(TChannelNb channelNb)
{
  // cmd, channel, lsb, msb
  return Packet_Put(CMD_ANALOG_VALUE, (UINT8)channelNb, Analog_Input[channelNb].Value.s.Lo, Analog_Input[channelNb].Value.s.Hi);  
}

//---------------------------------
//  HandleTestModePacket
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleTestModePacket(void)
{
  switch (Packet_Parameter3)
  {
    case 0:
      return Packet_Put( CMD_TEST_MODE, (UINT8)Debug, 0, 0 );
      break;
    case 1:
      return EEPROM_Write16(&Debug, Packet_Parameter1);    
      break;
  }
}

//---------------------------------
//  HandleTariffPacket
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleTariffPacket(void)
{
	switch (Packet_Parameter3)
	{
		case 0:
			return Packet_Put( CMD_TARIFF, (UINT8)sTariffNumber, 0, 0);
			break;
		case 1:
			return EEPROM_Write16(&sTariffNumber, Packet_Parameter1);
			break;
	}
}

//---------------------------------
//  HandleTime1Packet
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleTime1Packet(void)
{
  switch(Packet_Parameter3)
  {
    case 0:
      return Packet_Put(CMD_TIME1, System_Seconds.s.Lo, System_Minutes.s.Lo, 0);
      break;
    case 1:
      System_Seconds.s.Lo = Packet_Parameter1;
      System_Minutes.s.Lo = Packet_Parameter2;
      break;
  }
}

//---------------------------------
//  HandleTime2Packet
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleTime2Packet(void)
{
  switch(Packet_Parameter3)
  {
    case 0:
      return Packet_Put(CMD_TIME1, System_Hours.s.Lo, System_Days.s.Lo, 0);
      break;
    case 1:
      System_Hours.s.Lo = Packet_Parameter1;
      System_Days.s.Lo = Packet_Parameter2;
      break;
  }
}

//---------------------------------
//  HandlePowerPacket
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandlePowerPacket(void)
{
  switch(Packet_Parameter3)
  {
  	case 0:
  		return Packet_Put(CMD_POWER, DEM_Average_Power.s.Lo, DEM_Average_Power.s.Hi, 0);
  		break;
  }
}

//---------------------------------
//  HandleEnergyPacket
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleEnergyPacket(void)
{
  switch(Packet_Parameter1)
  {
    case 0:
      return Packet_Put(CMD_ENERGY, DEM_Total_Energy.s.Lo, DEM_Total_Energy.s.Hi, 0);
      break;
  }
}

//---------------------------------
//  HandleCostPacket
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleCostPacket(void)
{
  switch(Packet_Parameter1)
  {
    case 0:
      return Packet_Put(CMD_COST, DEM_Total_Cost.s.Lo, DEM_Total_Cost.s.Hi, 0); 
      break;
  }
}

//---------------------------------
//  HandleFrequencyPacket
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleFrequencyPacket(void)
{
  switch(Packet_Parameter1)
  {
  	case 0:
  		
  		break;
  	
  }
}

//---------------------------------
//  HandleVoltageRMSPacket
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleVoltageRMSPacket(void)
{
  switch(Packet_Parameter1)
  {
  	case 0:
  		Packet_Put(CMD_VOLTAGE_RMS, Voltage_RMS.s.Lo, Voltage_RMS.s.Hi, 0);
  		break;
  }
}

//---------------------------------
//  HandleCurrentRMSPacket
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleCurrentRMSPacket(void)
{
  switch(Packet_Parameter1)
  {
  	case 0:
  		Packet_Put(CMD_CURRENT_RMS, Current_RMS.s.Lo, Current_RMS.s.Hi, 0);
  		break;
  }
}

//---------------------------------
//  HandlePowerFactorPacket
//
//  
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandlePowerFactorPacket(void)
{
    switch(Packet_Parameter1)
  {
  	case 0:
  		
  		break;
  	
  }
}
