#include "stm32f4xx.h"
#include "stdio.h"
#include "math.h"

#include "LED.h"
#include "ADC.h"
#include "button.h"
#include "moving_average.h"

/// runs/flash
#define FLASH_PERIOD (1 * 20)

/// multiples of 50ms
#define DEBOUNCE_PERIOD 7

/// last letter controls function version: s = asm, c = C
#define MOVING_AVERAGE_X moving_average_s


typedef enum {
	TEMP_DISPLAY, 
	FLASHING
} Mode;

// incremented by interupt at 20Hz
static volatile uint_fast16_t ticks;

// uses LEDs to display temperature
static void displayTemp(float temp) {
	// debugging only
	printf("%f\n", temp);
	
	float normalized = fmod(temp - 2, 8);
	if(normalized < 2)
		LED_set(ON, OFF, OFF, OFF);
	else if (normalized < 4)
		LED_set(ON, ON, OFF, OFF);
	else if (normalized < 6)
		LED_set(ON, ON, ON, OFF);
	else
		LED_set(ON, ON, ON, ON);	
}

void SysTick_Handler() {
	ticks += 1;
}

// main program loop which is called at 20 Hz
// returns 1 on button push and 0 otherwise
static int run() {
	
	static int countToBlink = 0;
	static Mode currentMode = TEMP_DISPLAY;
	static FilterState filterState = {0};
	static LED_state flashState = ON;
	
	switch(currentMode) {
		case TEMP_DISPLAY:
			displayTemp(MOVING_AVERAGE_X(getTemp_celcius(), &filterState));
		break;
		case FLASHING:
			if(countToBlink != 0)
				countToBlink--;
			else {
				LED_setAll(flashState);
				countToBlink = FLASH_PERIOD;
				flashState = flashState ? OFF : ON;
			}
		break;
	}
	
	if( buttonIsPushed() ) {
		
		currentMode = currentMode? TEMP_DISPLAY : FLASHING;
		LED_setAll(OFF);
		countToBlink = FLASH_PERIOD;
		
		return DEBOUNCE_PERIOD;
		
	} else
		return 0;
}

int main() {
	int debouncing = 0;

	ticks = 0;
	
	ADC_init();
	LED_init();
	buttonInit();
	SysTick_Config(50 * SystemCoreClock / 1000);
		
	while(1) {
		// wait for interupt
		while(!ticks);
		
		ticks--;
		
		if(debouncing)
			debouncing--;
		else
			debouncing = run();
	}	
}



