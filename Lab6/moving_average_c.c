#include "moving_average.h"

// float moving_average(float newSample, FilterState* state) {
// 	float yNew = newSample + state->yPrevious - state->circBuf[state->tail];
// 	state->yPrevious = yNew;
// 	state->circBuf[state->tail] = newSample;
// 	state->tail = (state->tail + 1) % d;
// 	yNew /= (float)d;
// 	return yNew;
// }

int8_t moving_average(int8_t newSample, FilterState* state) {
	float yNew = newSample + state->yPrevious - state->circBuf[state->tail];
	state->yPrevious = yNew;
	state->circBuf[state->tail] = newSample;
	state->tail = (state->tail + 1) % d;
	yNew /= d;
	return yNew;
}
