// ----------------------------------------
// Filename: LCD.c
// Description: Lab 5
//  Implements the Clover LCD functions to control
//  backlighting, display and setup
// Author: Danny Sok
//         Edward Khou
// Date Created: 20-10-2011
// Date Last Modified: 2-11-2011

#include "LCD.h"
// LCD Function Settings
static UINT8 const SET_TEXT_HOME       = 0b01000000;
static UINT8 const SET_TEXTAREA        = 0b01000001;
static UINT8 const SET_CURSOR          = 0b00100001;
static UINT8 const SET_RAM             = 0b00100100;
static UINT8 const DISPLAY_MODE        = 0b10010111; // Text on, Graphics off, Curson on, Blink on
// 100101XX
// 1001XX11
static UINT8 const TXT_GFX_ON          = 0b10011000;
static UINT8 const SET_ONE_L           = 0b10100000;
static UINT8 const SET_TWO_L           = 0b10100001;
static UINT8 const SET_THREE_L         = 0b10100010;
static UINT8 const SET_FOUR_L          = 0b10100011;
static UINT8 const DATA_WRITE          = 0b11000000;
static UINT8 const DATA_READ           = 0b11000011;
static UINT8 const LCD_CLEAR           = 0b10010000;
static UINT8 const MODE_SET            = 0b10000001; // EXOR && Internal CG ROM
static UINT8 const CURSOR_SELECT       = 0b10100111; // 8-line cursor

static UINT8 const COLUMN_SIZE         = 16;


static UINT16 const Timer_Ch5_Delay    = 50000;     // ~20ms

// ----------------------------------------
// LCD_Setup
// 
// Sets up the LCD
// Input:
//   
// Output:
//   none
// Conditions:
//  none
void LCD_Setup(void)
{
  TTimerSetup timerSetup;
  //RESET
  LCDF_RST = 0;
  
  asm nop;
  asm nop;
  asm nop;   
  asm nop;
  asm nop;
  asm nop;
  
  LCDF_RST = 1;

  
  //DDRA = Input;
  
  LCD_SetFunction(BacklightON, (UINT8)sLCDContrast);

  // Default settings
  LCDF_WR = 1;
  LCDF_RD = 1; 
  
  // Set address pointer to home address
  SetRAMAddress(0);
  
  LCD_CursorHome();
  
  LCD_TextHome();
  LCD_TextArea(COLUMN_SIZE);

  // Set mode (EXOR mode and internal CG ROM)
  WriteCommand(MODE_SET);

  // Set display mode (Cursor ON, blink ON, Text ON, graphic OFF)
  WriteCommand(DISPLAY_MODE);
  
  // Cursor pattern select
  WriteCommand(CURSOR_SELECT);
  
  LCD_Clear();
   
  // Setup Channel 5 for Output Compare of LCD Status Checks
  timerSetup.outputCompare = bTRUE;
  timerSetup.outputAction = TIMER_OUTPUT_DISCONNECT;
  timerSetup.inputDetection = TIMER_INPUT_OFF;
  timerSetup.toggleOnOverflow = bFALSE;
  timerSetup.interruptEnable = bFALSE;

  // Setup and Enable the timer.  
  Timer_Init(TIMER_Ch5, &timerSetup);
  Timer_Set(TIMER_Ch5, Timer_Ch5_Delay);
  Timer_Enable(TIMER_Ch5, bTRUE);
}

// ----------------------------------------
// LCD_Clear
// 
// Clears the LCD
// Input:
//   none
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_Clear(void)
{  
  SetRAMAddress(0);
  
  /*INT8 i;
  
  for (i = 0; i < 8; i++)
  {
    LCD_ClearLine(i); 
  }*/
  
  LCD_ClearLine(0);
  LCD_ClearLine(1);
  LCD_ClearLine(2);
  LCD_ClearLine(3);
  LCD_ClearLine(4);
  LCD_ClearLine(5);
  LCD_ClearLine(6);
  LCD_ClearLine(7);
  
  SetRAMAddress(0);  
}

// ----------------------------------------
// LCD_ClearLine
// 
// Clears the LCD at the line specified
// Input:
//   line is the line position
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_ClearLine(const UINT8 line)
{
  LCD_SetLine(line);
  LCD_OutString("                "); 
}

// ----------------------------------------
// LCD_CursorHome
// 
// Homes the cursor
// Input:
//   none
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_CursorHome(void)
{
  // Set cursor pointer to home address
  SetCGAddress(0, 1);
}

// ----------------------------------------
// LCD_TextHome
// 
// Sets up the Cursor position to the start of the LCD
// Input:
//   none
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_TextHome(void)
{
  // Set text home address (start of RAM)
  WriteData(0x00);
  WriteData(0x00);
  WriteCommand(SET_TEXT_HOME);
}

// ----------------------------------------
// LCD_TextArea
// 
// Sets up the area available to use on the LCD
// Input:
//   column is the amount of columns to use e.g 16
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_TextArea(UINT8 const column)
{
  // Set text area (16 columns)
  WriteData(column);
  WriteData(0x00);
  
  WriteCommand(SET_TEXTAREA);
}

// ----------------------------------------
// LCD_OutChar
// 
// Writes a character to the LCD
// Input:
//   
// Output:
//   none
// Conditions:
//  none
BOOL LCD_OutChar(INT8 const data)
{
  if (data == '\0' || data == 0)
    return bFALSE;  
  
  WriteData(data - 0x20);
  WriteCommand(DATA_WRITE);
  return bTRUE;
}

// ----------------------------------------
// LCD_OutInteger
// 
// Writes a number to the LCD
// Input:
//   data is the number to write to the LCD
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_OutInteger(UINT16 const data)
{
  UINT16 number;
  UINT8 thousand, hundred, ten, single;
  thousand = 0;
  hundred  = 0;
  ten      = 0;
  single   = 0;
  number   = data;   
  
  if (number >= 1000) 
  {
    thousand = ((UINT8)number / 1000);
    number -= (thousand * 1000);
    thousand |= 0x30;
  }
  if (number >= 100)                       //if number is 300, 300/100 = 3
  {
    hundred = ((UINT8)number / 100);
    number -= (hundred * 100);             //wtfuggles?!?!?! must be a better way...
    hundred |= 0x30;                       //convert to ASCII by ORing with 0x30  
  }
  if (number >= 10)
  {
    ten = ((UINT8)number / 10);
    number -= (ten * 10);
    ten |= 0x30;  
  }
  if (number >= 0)
  {
    single = (UINT8)number;
    single |= 0x30; 
  }
    
  if (hundred == 0 && thousand != 0)
    hundred |= 0x30;
  
  if (ten == 0 && hundred != 0)
    ten |= 0x30;
  
  (void)LCD_OutChar(thousand);
  (void)LCD_OutChar(hundred);
  (void)LCD_OutChar(ten);
  (void)LCD_OutChar(single);
}

// ----------------------------------------
// LCD_OutString
// 
// Writes a character to the LCD
// Input:
//   data is the string to write to the LCD
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_OutString(INT8 * const data)
{
  INT8 i;
  for (i = 0; i < COLUMN_SIZE; i++)
  {
    if ( !LCD_OutChar(data[i]) )
      break;
  }
}

// ----------------------------------------
// LCD_SetFunction
// 
// Sets the function of the LCD
// Will setup the Contrast and Backlight
// Input:
//   
// Output:
//   none
// Conditions:
//  none
void LCD_SetFunction(const TLCDBacklight backlight, const INT8 contrast)
{
  INT8 i;
  
  LCDF_CONTRAST_CTRL   = 1;
  LCDF_CONTRAST_ADJ    = 1;
  LCDF_CONTRAST_CTRL   = 0;
  LCDF_CONTRAST_ADJ    = 0;
  LCDF_CONTRAST_CTRL   = 1;
  
  // Set the contrast
  for (i = 0; i < contrast; i++)
  { 
     LCDF_CONTRAST_ADJ = 1;
     LCDF_CONTRAST_ADJ = 0;
  }
  
  LCDF_BACKLIGHT = (UINT8)backlight;
}

// ----------------------------------------
// LCD_SetLine
// 
// Sets the current line of the LCD
// Input:
//   
// Output:
//   none
// Conditions:
//  none
void LCD_SetLine(const UINT8 line)
{  
  // This is due to the fact that each new line starts
  // after the amount of columns
  // i.e new line after 16 columns
  // then another, etc
  SetRAMAddress(COLUMN_SIZE * line);
}

// ----------------------------------------
// LCD_SetCursor
// 
// Sets the current cursor position of the LCD
// Input:
//   x is the x address (column)
//   y is the y address (row)
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_SetCursor(const UINT8 x, const UINT8 y)
{
  SetCGAddress(x, y);
}

// ----------------------------------------
// InChar
// 
// Reads data from the LCD
// Input:
//   dataPtr is a pointer to the variable in which the data from
//   PORTA will be stored
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void InChar(UINT8 * const dataPtr)
{
  DDRA = Output;
  WriteCommand(DATA_READ);  
  
  DDRA = Input;
  LCDF_CD = eData;
  
  *dataPtr = PORTA;
  DDRA = Output;
}

// ----------------------------------------
// SetCGAddress
// 
// Sets the character generator address
// Input:
//   x is the x address (column)
//   y is the y address (row)
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void SetCGAddress(const UINT8 x, const UINT8 y)
{
  WriteData(x);
  WriteData(y);
  WriteCommand(SET_CURSOR);
}

// ----------------------------------------
// SetRAMAddress
// 
// Sets the RAM address of the LCD
// Input:
//   address is the pointer to a 16 bit address to point to the location
//   of the ram on the LCD
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void SetRAMAddress(const UINT16 address)
{
  TUINT16 address16;
  address16.l = address;
  WriteData(address16.s.Lo);
  WriteData(address16.s.Hi);  
  WriteCommand(SET_RAM);
}

// ----------------------------------------
// StatusCheck
// 
// Determines if it is safe to read/write to the LCD
// Input:
//   none
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
UINT8 StatusCheck(void)
{ 
  UINT8 status;
 
  //Default settings 
  DDRA = Input;
  LCDF_WR = 1;
  LCDF_RD = 1;
  
  LCDF_CD = eCommand;
  LCDF_RD = 0;
 
  // Wait at least 150ns to read data
  asm nop;
  asm nop;
  asm nop;
  asm nop;
  asm nop;
 
  status = PORTA;
  
  //Put LCD back in default setting 
  LCDF_RD = 1;
  
  return (status & 0x03); 
}

// ----------------------------------------
// WaitForReady
// 
// Waits for the LCD to become ready 
// Input: 
//   None 
// Output: 
//   None
// Conditions: 
//   Assumes LCD_Setup has been called

void WaitForReady(void)
{
  // Wait until STA0 & STA1 are set
  while ( (StatusCheck()) != 0x03);
  
  
}

// ----------------------------------------
// WriteCommand
// 
// Writes a command to the LCD (when LCD is ready to accept)
// Input: 
//   an LCD command 
// Output: 
//   None
// Conditions: 
//   Assumes LCD_Setup has been called

void WriteCommand(const UINT8 command)          
{
  WaitForReady();
  
  DDRA = Output;
  
  PORTA = command;
  LCDF_CD = eCommand;
  LCDF_WR = 0;
  
  // Wait at least 80ns (data set-up time)
  asm nop;
  asm nop;
  asm nop;
  
  LCDF_WR = 1;
  DDRA = Input;

}

// ----------------------------------------
// WriteData
// 
// Writes 1 byte of data to the LCD  
// Input: 
//   Data to be written 
// Output: 
//   None
// Conditions: 
//   Assumes LCD_Setup has been called

void WriteData(const UINT8 data)
{
  WaitForReady();
  
  DDRA = Output;
  
  PORTA = data;
  LCDF_CD = eData;
  LCDF_WR = 0;
    
  // Wait at least 80ns (data set-up time)
  asm nop;
  asm nop;
  asm nop;
  
  LCDF_WR = 1;
  DDRA = Input;
}
