// ----------------------------------------
// Filename: HMI.c
// Description: Lab 5
//  Implements the HMI to control the LCD and menu system
//  backlighting, display and setup
// Author: Danny Sok
//         Edward Khou
// Date Created: 20-10-2011
// Date Last Modified: 2-11-2011

#include "HMI.h"

// Position of the cursors
static UINT8 X, Y;
static BOOL ItemSelected;

static UINT8   TempContrast; 
static UINT8   TempBacklight;
static UINT16  TempMCNb; 
static UINT8   TempProtMode;
static UINT8   TempDebug;
static UINT32  Interval;

static TLCDState LCDPrevState, LCDCurrentState;
static TButtonInputs PBState, PrevPBState;

// ----------------------------------------
// HMI_Setup
// 
// Sets up the HMI
// Input:
//   
// Output:
//   none
// Conditions:
//  none
void HMI_Setup(void)
{
  // Port K - Push Buttons
  DDRK_BIT2 = 0;    // 0: Input, PK2
  DDRK_BIT3 = 0;    // 0: Input, PK3
  DDRK_BIT4 = 0;    // 0: Input, PK4
  DDRK_BIT5 = 0;    // 0: Input, PK5
  DDRK_BIT7 = 0;    // 0: Input, PK7
  
  // Port A: LCD Display
  DDRA_BIT0 = 1;    // 1: Output, PA0
  DDRA_BIT1 = 1;    // 1: Output, PA1
  DDRA_BIT2 = 1;    // 1: Output, PA2
  DDRA_BIT3 = 1;    // 1: Output, PA3
  DDRA_BIT4 = 1;    // 1: Output, PA4
  DDRA_BIT5 = 1;    // 1: Output, PA5
  DDRA_BIT6 = 1;    // 1: Output, PA6
  DDRA_BIT7 = 1;    // 1: Output, PA7
  
  // Port B: LCD Control
  DDRB_BIT0 = 1;    // 1: Output, PB0
  DDRB_BIT1 = 1;    // 1: Output, PB1
  DDRB_BIT2 = 1;    // 1: Output, PB2
  DDRB_BIT3 = 1;    // 1: Output, PB3
  DDRB_BIT4 = 1;    // 1: Output, PB4
  DDRB_BIT5 = 1;    // 1: Output, PB5
  DDRB_BIT6 = 1;    // 1: Output, PB6
  DDRB_BIT7 = 1;    // 1: Output, PB7
  
  LCD_Setup();
  X, Y = 0;
  CreateMenu(DormantMenu);
  
  TempContrast   = (UINT8)sLCDContrast;
  TempBacklight  = LCDF_BACKLIGHT;
  TempMCNb       = sModConNb;
  TempProtMode   = (UINT8)sControlMode;
  TempDebug      = (UINT8)Debug;
  Interval       = 0;

  ItemSelected = bFALSE;
  LCDPrevState = DormantMenu;
}

// ----------------------------------------
// HMI_Update
// 
// Updates the HMI
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void HMI_Update(void)
{
  CreateMenu(LCDCurrentState);
}

// ----------------------------------------
// TIE5_ISR
// 
// The interrupt service routine for Timer Channel 5.
// It will determine if a pushbutton has been pressed and will
// allow for the button event to be handled.
// Input:
//   
// Output:
//   none
// Conditions:
//  none
void interrupt 13 TIE5_ISR(void)
{
  TFLG1_C5F = 1; // Clear flag to say it was done.
  
  Buttons_Get(&PBState); 
  
  if (PBState != PrevPBState)
  {
    HandleButtonPress(PBState, LCDPrevState);
    
    Interval = 0;
  }
  else if (PBState == ButtonIdle && PBState == PrevPBState)
    Interval += 50000; // 50k clock cycles
    
  PrevPBState = PBState;
  if (Interval >= 240000000) // 10 Seconds: 240Million Clock Cycles
  {
    //CreateMenu(DormantMenu);
    //CreateMenu(DefaultMenu);
    RevertSettings();
    Interval = 0;
  }
    
}

// ----------------------------------------
// CreateMenu
// 
// Creates the menu to display on the LCD
// Input:
//   menu is the state to display on the LCD e.g. DefaultMenu
// Output:
//   none
// Conditions:
//   LCD and HMI have been setup
void CreateMenu(TLCDState menu)
{
  LCD_SetCursor(0, 1); // Reset Cursor
  X = 0;
  Y = 1;
  LCD_Clear();
  
  switch(menu)
  {
    case DormantMenu:
      LCD_SetFunction(BacklightOFF, (UINT8)sLCDContrast);
      LCD_SetCursor(-1, -1);
      /*LCD_SetLine(0);
      LCD_OutString("                ");
      LCD_SetLine(1);
      LCD_OutString("                ");
      LCD_SetLine(2);
      LCD_OutString("                ");
      LCD_SetLine(3);
      LCD_OutString("                ");
      LCD_SetLine(4);
      LCD_OutString("                ");
      LCD_SetLine(5);
      LCD_OutString("                ");
      LCD_SetLine(6);
      LCD_OutString("                ");
      LCD_SetLine(7);
      LCD_OutString("    DORMANT     ");*/
      LCDCurrentState = DormantMenu;
    break;
    
    case MeteringTimeMenu:
    	LCD_SetFunction(BacklightON, (UINT8)sLCDContrast);
      LCD_SetLine(0);
      LCD_OutString("METERING TIME   ");
      LCD_SetLine(1);
      if (System_Days.s.Lo >= 99)
        LCD_OutString("xx:xx:xx:xx");
      else
      {
        LCD_OutInteger(System_Days.s.Lo);
        LCD_OutString(":");
        LCD_OutInteger(System_Hours.s.Lo);
        LCD_OutString(":");
        LCD_OutInteger(System_Minutes.s.Lo);
        LCD_OutString(":");
        LCD_OutInteger(System_Seconds.s.Lo);
      }
      
      LCD_SetLine(2);
      LCD_OutString("                ");
      LCD_SetLine(3);
      LCD_OutString("                ");
      LCD_SetLine(4);
      LCD_OutString("                ");
      LCD_SetLine(5);
      LCD_OutString("Test Mode: ");
      if (Debug)
        LCD_OutString("ON ");
      else
        LCD_OutString("OFF");
      LCD_SetLine(6);
      LCD_OutString("Rate: ");
      Calculate_TariffQNotation(sCurrentTariffRate);
      LCD_SetLine(7);
      LCD_OutString("      CYC       ");
      LCDCurrentState = MeteringTimeMenu;    
    break;
        
    case AveragePowerMenu:
      LCD_SetLine(0);
      LCD_OutString("AVERAGE POWER   ");
      LCD_SetLine(1);
      LCD_OutString("                ");
      LCD_SetLine(2);
      LCD_OutString("                ");
      LCD_SetLine(3);
      LCD_OutString("                ");
      LCD_SetLine(4);
      LCD_OutString("                ");
      LCD_SetLine(5);
      LCD_OutString("Test Mode: ");
      if (Debug)
        LCD_OutString("ON ");
      else
        LCD_OutString("OFF");
      LCD_SetLine(6);
      LCD_OutString("Rate: ");
      Calculate_TariffQNotation(sCurrentTariffRate);
      LCD_SetLine(7);
      LCD_OutString("      CYC       ");
      LCDCurrentState = AveragePowerMenu;    
    break;
        
    case TotalEnergyMenu:
      LCD_SetLine(0);
      LCD_OutString("TOTAL ENERGY   ");
      LCD_SetLine(1);
      if (Total_Energy.s.Lo >= 999)
      	LCD_OutString("xxx:xxx");
      else
      {
      	LCD_OutInteger(Total_Energy.s.Hi);
      	LCD_OutString(".");
      	LCD_OutInteger(Total_Energy.s.Lo);
      	LCD_OutString(" kWh");
      }
      LCD_SetLine(2);
      LCD_OutString("                ");
      LCD_SetLine(3);
      LCD_OutString("                ");
      LCD_SetLine(4);
      LCD_OutString("                ");
      LCD_SetLine(5);
      LCD_OutString("Test Mode: ");
      if (Debug)
        LCD_OutString("ON ");
      else
        LCD_OutString("OFF");
      LCD_SetLine(6);
      LCD_OutString("Rate: ");
      //LCD_QNotation(sCurrentTariffRate);
      LCD_SetLine(7);
      LCD_OutString("      CYC       ");
      LCDCurrentState = TotalEnergyMenu;
    break;
        
    case TotalCostMenu:
      LCD_SetLine(0);
      LCD_OutString("TOTAL COST      ");
      LCD_SetLine(1);
      if (Total_Cost.s.Hi >= 9999 && Total_Cost.s.Lo >=99)
      	LCD_OutString("xxxx.xx");
      else
      {
      	LCD_OutString("$");
      	LCD_OutInteger(Total_Cost.s.Hi);
      	LCD_OutString(".");
      	LCD_OutInteger(Total_Cost.s.Lo);
      }
      LCD_OutString("                ");
      LCD_SetLine(2);
      LCD_OutString("                ");
      LCD_SetLine(3);
      LCD_OutString("                ");
      LCD_SetLine(4);
      LCD_OutString("                ");
      LCD_SetLine(5);
      LCD_OutString("Test Mode: ");
      if (Debug)
        LCD_OutString("ON ");
      else
        LCD_OutString("OFF");
      LCD_SetLine(6);
      LCD_OutString("Rate: ");
      Calculate_TariffQNotation(sCurrentTariffRate);
      LCD_SetLine(7);
      LCD_OutString("      CYC       ");
      LCDCurrentState = TotalCostMenu;
    break;
    
    /////////////////////////////////////////////////////  
    
    case DefaultMenu:
      LCD_SetLine(0);
      //////////////"                "//
      //////////////"1234567890123456"//
      LCD_OutString("ES Group 13 MENU");
      LCD_SetLine(1);
      LCD_OutString(" ModCon Nb:");
      LCD_OutInteger( sModConNb );
      LCD_SetLine(2);
      LCD_OutString(" Version:  V1.0 ");
      LCD_SetLine(3);
      LCD_OutString(" Debug:    ");
      if (Debug)
        LCD_OutString("ON ");
      else
        LCD_OutString("OFF");
      LCD_SetLine(4);
      LCD_OutString(" Prot Mode:");
      if (sControlMode == PACKET_ASYNCHRONOUS)
        LCD_OutString("ASYN ");
      else
        LCD_OutString("SYNC ");
      LCD_SetLine(5);
      LCD_OutString("                ");
      LCD_SetLine(6);
      LCD_OutString("                ");
      LCD_SetLine(7);
      LCD_OutString("      MENU      ");
    break;
    
    case SetMenu:
      LCD_SetLine(0);
      LCD_OutString("  Settings MENU ");
      LCD_SetLine(1);
      LCD_OutString(" ModCon Settings");
      LCD_SetLine(2);
      LCD_OutString(" LCD Settings   ");
      LCD_SetLine(3);
      LCD_OutString("                ");
      LCD_SetLine(4);
      LCD_OutString("                ");
      LCD_SetLine(5);
      LCD_OutString("                ");
      LCD_SetLine(6);
      LCD_OutString("                ");
      LCD_SetLine(7);
      LCD_OutString("UP DN    SEL BAK");
    break;
    
    case LCDSetMenu:
      LCD_SetLine(0);
      LCD_OutString(" LCD Settings   ");
      LCD_SetLine(1);
      LCD_OutString(" Contrast:   ");
      LCD_OutInteger( sLCDContrast );
      LCD_SetLine(2);
      LCD_OutString(" Backlight:  ");
      if (LCDF_BACKLIGHT)
        LCD_OutString("ON ");
      else
        LCD_OutString("OFF");
      LCD_SetLine(3);
      LCD_OutString("                ");
      LCD_SetLine(4);
      LCD_OutString("                ");
      LCD_SetLine(5);
      LCD_OutString("                ");
      LCD_SetLine(6);
      LCD_OutString("                ");
      LCD_SetLine(7);
      LCD_OutString("UP DN CY SEL SAV");
    break;
    
    case ModConSetMenu:
      LCD_SetLine(0);
      LCD_OutString(" ModCon Settings");
      LCD_SetLine(1);
      LCD_OutString(" ModCon Nb:");
      LCD_OutInteger(sModConNb);
      LCD_SetLine(2);
      LCD_OutString(" Prot Mode:");
      if (sControlMode == PACKET_ASYNCHRONOUS)
        LCD_OutString("ASYN ");
      else
        LCD_OutString("SYNC ");
      LCD_SetLine(3);
      LCD_OutString(" Debug:    ");
      if (Debug)
        LCD_OutString("ON ");
      else
        LCD_OutString("OFF");
      LCD_SetLine(4);
      LCD_OutString("                ");
      LCD_SetLine(5);
      LCD_OutString("                ");
      LCD_SetLine(6);
      LCD_OutString("                ");
      LCD_SetLine(7);
      LCD_OutString("UP DN CY SEL SAV");
    break;
    
    case SaveMenu:
      LCD_SetLine(0);
      LCD_OutString("ES Group 13 MENU");
      LCD_SetLine(1);
      LCD_OutString("                ");
      LCD_SetLine(2);
      LCD_OutString("Would you like  ");
      LCD_SetLine(3);
      LCD_OutString("to save your    ");
      LCD_SetLine(4);
      LCD_OutString("changes?        ");
      LCD_SetLine(5);
      LCD_OutString("                ");
      LCD_SetLine(6);
      LCD_OutString("                ");
      LCD_SetLine(7);
      LCD_OutString("YES           NO");
    break;
    
    case SaveSuccessfulMenu:
      LCD_SetLine(0);
      LCD_OutString("ES Group 13 MENU");
      LCD_SetLine(1);
      LCD_OutString("                ");
      LCD_SetLine(2);
      LCD_OutString("Your new setting");
      LCD_SetLine(3);
      LCD_OutString("have been saved ");
      LCD_SetLine(4);
      LCD_OutString("successfully    ");
      LCD_SetLine(5);
      LCD_OutString("                ");
      LCD_SetLine(6);
      LCD_OutString("                ");
      LCD_SetLine(7);
      LCD_OutString("                ");
    break;
  }
  LCDPrevState = LCDCurrentState;
}

// ----------------------------------------
// HandleButtonPress
// 
// Determines which functions to call/what to do when a button is pressed
// Input:
//   PBState is the push button state that was most recent
//   LCDState is the menu currently displayed on the LCD
// Output:
//   none
// Conditions:
//   LCD and Buttons have been setup
void HandleButtonPress(TButtonInputs PBState, TLCDState LCDState)
{
  switch(LCDState)
  {
    case DormantMenu:
      switch(PBState)
      {
        case Button2:
          CreateMenu(MeteringTimeMenu);
        break;
      }
    break;
    
    case MeteringTimeMenu:
      switch(PBState)
      {
        case Button2:
          CreateMenu(AveragePowerMenu);
        break;
      }
    break;
    
    case AveragePowerMenu:
      switch(PBState)
      {
        case Button2:
          CreateMenu(TotalEnergyMenu);
        break;
      }
    break;
    
    case TotalEnergyMenu:
      switch(PBState)
      {
        case Button2:
          CreateMenu(TotalCostMenu);
        break;
      }
    break;
    
    case TotalCostMenu:
      switch(PBState)
      {
        case Button2:
          CreateMenu(MeteringTimeMenu);
        break;
      }
    break;
    
    /////////////////////////////////////////////////
    
    case DefaultMenu:
      switch(PBState)
      {
        case Button2:
          CreateMenu(SetMenu);
        break;
      }
    break;
      
    case SetMenu:
      switch(PBState)
      {
        case Button0:
          MoveUp();
        break;
        case Button1:
          MoveDown();
        break;
        case Button3:
          SelectItem(LCDState);
        break;
        case Button4:
          CreateMenu(DefaultMenu);
        break;
      }
    break;
      
    case LCDSetMenu:
      switch (PBState)
      {
        case Button0:
          MoveUp();
        break;
        case Button1:
          MoveDown();
        break;
        case Button2:
          CycleData(LCDState);
        break;
        case Button3:
          SelectItem(LCDState);
        break;
        case Button4:
          CreateMenu(SaveMenu);
        break;
      }
    break;
    
    case ModConSetMenu:
      switch (PBState)
      {
        case Button0:
          MoveUp();
        break;
        case Button1:
          MoveDown();
        break;
        case Button2:
          CycleData(LCDState);
        break;
        case Button3:
          SelectItem(LCDState);
        break;
        case Button4:
          CreateMenu(SaveMenu);
        break;
      }
    break;
    
    case SaveMenu:
      switch (PBState)
      {
        case Button0:   // Yes
          // ModCon Settings Save
          (void)EEPROM_Write16(&Debug, TempDebug);
          (void)EEPROM_Write16(&sControlMode, TempProtMode);
          (void)EEPROM_Write16(&sModConNb, TempMCNb);
          
          
                                                                      
          // LCD Settings Save
          LCDF_BACKLIGHT = TempBacklight; 
          (void)EEPROM_Write16(&sLCDContrast, TempContrast);
          LCD_SetFunction(LCDF_BACKLIGHT, (UINT8)sLCDContrast);
          
          CreateMenu(SaveSuccessfulMenu);
          CreateMenu(DefaultMenu);
        break;
        
        case Button4:   // No
          CreateMenu(DefaultMenu);
          RevertSettings();
          
        break;
      }
    break;
    
  }
  
}

// ----------------------------------------
// CycleData
// 
// Cycles through the data on the LCD e.g ModConNb from 1 to 65535
// Input:
//   LCDState is the menu currently displayed on the LCD
// Output:
//   none
// Conditions:
//   none
void CycleData(TLCDState LCDState)
{
  switch (LCDState)
  {
    // We only need to write for the lines that we want to
    // cycle, other wise it's a giant waste of time/effort/power
    case LCDSetMenu:
      if (Y == Line1 && ItemSelected) // CONTRAST
      {
        /////////////("////////////////");
        LCD_ClearLine(1);
        LCD_SetLine(1); 
        LCD_OutString(" Contrast:   ");
        LCD_OutInteger(++TempContrast);
        
        if (TempContrast == 100)
          TempContrast = 0;
      }
      
      if (Y == Line2 && ItemSelected) // BACKLIGHT
      {
        TempBacklight ^= 1; // Toggle Backlight
        
        LCD_ClearLine(2);
        LCD_SetLine(2); 
        LCD_OutString(" Backlight:  ");
        if (TempBacklight)
          LCD_OutString("ON ");
        else
          LCD_OutString("OFF");        
      }
    break;
    
    case ModConSetMenu:
      if (Y == Line1 && ItemSelected) // Number
      {
        LCD_ClearLine(1);
        LCD_SetLine(1); 
        LCD_OutString(" ModCon Nb:");
        LCD_OutInteger(++TempMCNb);
        
        if (TempMCNb == 0xFFFF)
          TempMCNb = 0;       
      }
      
      if (Y == Line2 && ItemSelected) // Prot Mode
      {
        TempProtMode ^= 1; // Toggle Protocol Mode
        
        LCD_ClearLine(2);
        LCD_SetLine(2);
        LCD_OutString(" Prot Mode:");
        if (TempProtMode == PACKET_ASYNCHRONOUS)
          LCD_OutString("ASYN ");
        else
          LCD_OutString("SYNC ");
        
        
      }
      
      if (Y == Line3 && ItemSelected) // Debug
      {
        TempDebug ^= 1; // Toggle Debug\
        
        LCD_ClearLine(3);
        LCD_SetLine(3);
        LCD_OutString(" Debug:    ");
        if (TempDebug)
          LCD_OutString("ON ");
        else
          LCD_OutString("OFF");        
      }
    break;
    
  }
}

void RevertSettings(void)
{
  TempContrast   = (UINT8)sLCDContrast;
  TempBacklight  = LCDF_BACKLIGHT;
  TempMCNb       = sModConNb;
  TempProtMode   = (UINT8)sControlMode;
  TempDebug      = (UINT8)Debug;   
}

// ----------------------------------------
// MoveUp
// 
// Moves the cursor up
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void MoveUp(void)
{
  if (!ItemSelected)
  {
    Y--;
    
    if (Y < 1)
      Y = 1;
    
    LCD_SetCursor(X, Y);
  }
}

// ----------------------------------------
// MoveDown
// 
// Moves the cursor down
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void MoveDown(void)
{
  if (!ItemSelected)
  {
    Y++;
    
    if (Y > 6)
      Y = 6;
    
    LCD_SetCursor(X, Y);
  }
}

// ----------------------------------------
// SelectItem
// 
// Selects the current item the cursor is on
// Input:
//   LCDState is the menu currently displayed on the LCD
// Output:
//   none
// Conditions:
//   none
void SelectItem(TLCDState LCDState)
{
  // How are we going to do this lol.
  // I guess use the cursor y address to figure out the line
  // and then if the line matches a selectable point do something?
  switch (LCDState)
  {
    case SetMenu:
      if (Y == Line1)// ModCon
      {
        CreateMenu(ModConSetMenu);  
      }
      if (Y == Line2) // LCD
      {
        CreateMenu(LCDSetMenu);
      }
    break;
    
    case LCDSetMenu:
      if ((Y == Line1) || (Y == Line2)) // CONTRAST || BACKLIGHT
      {
        ItemSelected ^= bTRUE;  // Toggle Selection        
      }
    break;
    
    case ModConSetMenu:
      if ((Y == Line1) || (Y == Line2) || (Y == Line3)) // Number || Prot Mode || Debug
      {
        ItemSelected ^= bTRUE;  // Toggle Selection  
      }
    break;
  }
}

