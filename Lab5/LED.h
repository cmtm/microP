#pragma once

/// @file LED.h

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/// abstract representation of LED state
typedef enum {
	OFF,
	ON
} LED_state;

/// Initialize LED functionality
///
/// This must be called before LED_set*()
void LED_init(void);

/// set LED states
///
/// @param s0 LED_0 state
/// @param s1 LED_1 state
/// @param s2 LED_2 state
/// @param s3 LED_3 state
void LED_set(LED_state s0, LED_state s1, LED_state s2, LED_state s3);

void LED_set_ar(LED_state* ss);

/// sets all LED state to a single value
///
/// @param s state all LEDs will take
void LED_setAll(LED_state s);
