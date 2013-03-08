#pragma once

/// @file moving_average.h

/// window depth
#define d 5

/// moving_average's state
typedef struct {
	float circBuf[d];
	int tail;
	float yPrevious;	
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
float moving_average(float newSample, FilterState* state);

