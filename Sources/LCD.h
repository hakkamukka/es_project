// ----------------------------------------
// Filename: LCD.h
// Description: Lab 5
//  Implements the Clover LCD functions to control
//  backlighting, display and setup
// Author: Danny Sok
//         Edward Khou
// Date Created: 20-10-2011
// Date Last Modified: 2-11-2011

#ifndef LCD_H
#define LCD_H

#include <mc9s12a256.h>     /* derivative information */
#include "types.h"
#include "timer.h"
#include "EEPROM.h"

#define LCDF_WR             PORTB_BIT0
#define LCDF_RD             PORTB_BIT1
#define LCDF_CD             PORTB_BIT2
#define LCDF_RST            PORTB_BIT3
#define LCDF_CONTRAST_ADJ   PORTB_BIT4
#define LCDF_CONTRAST_CTRL  PORTB_BIT5
#define LCDF_BACKLIGHT      PORTB_BIT6



// LCD State Control
typedef enum 
{
  DefaultMenu,
  SetMenu,
  SaveMenu,
  LCDSetMenu,
  ModConSetMenu,
  SaveSuccessfulMenu,
  SaveUnsuccessfulMenu,
  
  DormantMenu,
  MeteringTimeMenu,
  AveragePowerMenu,
  TotalEnergyMenu,
  TotalCostMenu
} TLCDState;

// Line Y position (row)
typedef enum
{
  Line0 = 0,
  Line1 = 1,
  Line2 = 2,
  Line3 = 3,
  Line4 = 4,
  Line5 = 5,
  Line6 = 6,
  Line7 = 7
} TLCDLinePosition;

// LCD Backlighting Control
typedef enum
{
  BacklightON = 1,
  BacklightOFF = 0  
} TLCDBacklight;

// Port Direction (Input or Output) for global setting
typedef enum
{
  Output = 0xFF,
  Input = 0
} TPortDirection;

// Tells the LCD whether the data sent is a command or data
typedef enum
{
  eCommand = 1,
  eData = 0
} TLCDDataCommand;





// ----------------------------------------
// LCD_Setup
// 
// Sets up the LCD
// Input:
//   none
// Output:
//   none
// Conditions:
//  none
void LCD_Setup(void);

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
void LCD_Clear(void);

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
void LCD_ClearLine(const UINT8 line);

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
void LCD_CursorHome(void);

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
void LCD_TextHome(void);

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
void LCD_TextArea(const UINT8 column);

// ----------------------------------------
// LCD_OutChar
// 
// Writes a character to the LCD
// Input:
//   data is the character/letter to write to the LCD
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
BOOL LCD_OutChar(const INT8 data);

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
void LCD_OutInteger(const UINT16 data);

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
void LCD_OutString(INT8 * const data);

// ----------------------------------------
// LCD_SetFunction
// 
// Sets the function of the LCD
// Input:
//   backlight is the boolean to determine if the backlight is on or off
//   contrast is the percentage of contrast to set the LCD to
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_SetFunction(const TLCDBacklight backlight, const INT8 contrast);

// ----------------------------------------
// LCD_SetLine
// 
// Sets the current line of the LCD
// Input:
//   line is the line number to set the LCD Cursor to
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void LCD_SetLine(const UINT8 line);

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
void LCD_SetCursor(const UINT8 x, const UINT8 y);

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
void InChar(UINT8 * const dataPtr);

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
void SetCGAddress(const UINT8 x, const UINT8 y);

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
void SetRAMAddress(const UINT16 address);

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
UINT8 StatusCheck(void);

// ----------------------------------------
// WaitForReady
// 
// Waits for the LCD to become ready
// Input:
//   none
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void WaitForReady(void);

// ----------------------------------------
// WriteCommand
// 
// Writes a command to the LCD
// Input:
//   command is the command to write to the LCD (PORTA) data lines
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void WriteCommand(const UINT8 command);

// ----------------------------------------
// WriteData
// 
// Writes data to the LCD e.g the RAM address
// Input:
//   data is the data to write to the LCD
// Output:
//   none
// Conditions:
//   LCD_Setup() has been run
void WriteData(const UINT8 data);




#endif