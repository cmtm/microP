#include "moving_average.h"

float moving_average_c(float newSample, FilterState* state) {
	float yNew = newSample + state->yPrevious - state->circBuf[state->tail];
	state->yPrevious = yNew;
	state->circBuf[state->tail] = newSample;
	state->tail = (state->tail + 1) % d;
	yNew /= (float)d;
	return yNew;
}
