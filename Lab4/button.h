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

