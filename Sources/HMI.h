// ----------------------------------------
// Filename: HMI.h
// Description: Lab 5
//  Implements the HMI to control the LCD and menu system
//  backlighting, display and setup
// Author: Danny Sok
//         Edward Khou
// Date Created: 20-10-2011
// Date Last Modified: 2-11-2011

#ifndef HMI_H
#define HMI_H

#include "types.h"
#include "LCD.h"
#include "buttons.h"
#include "EEPROM.h"

#include <mc9s12a256.h>     /* derivative information */



// ----------------------------------------
// HMI_Setup
// 
// Sets up the HMI
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void HMI_Setup(void);

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
void CreateMenu(TLCDState menu);

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
void HandleButtonPress(TButtonInputs PBState, TLCDState LCDState);

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
void CycleData(TLCDState LCDState);

// ----------------------------------------
// RevertSettings
// 
// Reverts the Temporary settings back to normal
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void RevertSettings(void);

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
void MoveUp(void);

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
void MoveDown(void);

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
void SelectItem(TLCDState LCDState);


#endif