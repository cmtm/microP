#include <stdio.h>
#include <math.h>
#define d 10

// --- External functions
void moving_average_init(void* state);
float moving_average(float newSample, void* state);

// --- State struct
typedef struct {
	float circBuf[d];
	int tail;
	float yPrevious;	
} State;

// --- testing function
int moving_average_test(float* signal) {
	State st;
	moving_average_init(&st);
	float y;
	
	for(int i=0; i < 512; i++, signal++) {
		y = moving_average(*signal, &st);
		
		// non zero return means there was an error
		if (!isnormal(y)) {
			if (isinf(y))
				return 1;
			else if (isnan(y))
				return 2;
			else
				return 3;
		}
			
		printf("%d; %f; %f\n", i, *signal, y);
	}
	
	return 0;
}

// --- main isn't used
int main() {
	return 0;
}
