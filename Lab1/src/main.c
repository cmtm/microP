#include <stdio.h>
#include <math.h>
#define d 5
//-------------------------------------------------------------------
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle; /* Add whatever needed */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
//-------------------------------------------------------------------
typedef struct {
	float circBuf[d];
	int tail;
	float yPrevious;
	
	
} State;

void moving_average_init(void* state);
float moving_average(float newSample, void* state);

int moving_average_test(float* signal) {
	State st;
	moving_average_init(&st);
	float y;
	
	for(int i=0; i < 512; i++, signal++) {
		y = moving_average(*signal, &st);
		
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
	
int main() {
	return 0;
}
