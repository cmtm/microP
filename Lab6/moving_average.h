#pragma once

#include <stdint.h>

/// @file moving_average.h

/// window depth
#define d 10

/// moving_average's state
// typedef struct {
// 	float circBuf[d];
// 	int tail;
// 	float yPrevious;	
// } FilterState;

typedef struct {
	int16_t circBuf[d];
	int tail;
	int16_t yPrevious;	
} FilterState;

/// filters input with a moving average function
///
/// The samples are passed one value at a time.
/// It uses the FilterState structure to remember
/// state between calls.
///
/// @param newSample latest signal sample
/// @param state state of filter computation
/// @return latest sample of filtered signal
int8_t moving_average(int8_t newSample, FilterState* state);

