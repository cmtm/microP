
#define d 10

typedef struct {
	float circBuf[d];
	int tail;
	float yPrevious;	
} FilterState;

float moving_average_s(float newSample, FilterState* state);
float moving_average_c(float newSample, FilterState* state);
