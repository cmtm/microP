#pragma once

/// @file button.h

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


/// Initialize button functionality
///
/// This must be called before buttonIsPushed()
void buttonInit(void);

/// Check if button is pressed
///
/// @return 1 if button is pressed, 0 otherwise
int buttonIsPushed(void);

/// Check if button press occured
///
/// This function maintains state. If continuously polled, 
/// this function will return 1 when the button rises after 
/// a button press occured. It must be called when the button 
/// is held down (buttonIsPushed() returns 1) for it to work 
/// (note that it will return 0 in this case though).
///
/// @return 1 on button rise, 0 otherwise
int buttonWasPushed(void);

