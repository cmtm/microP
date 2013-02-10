#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stdio.h"
#include "math.h"

#include "LED.h"
#include "ADC.h"
#include "button.h"
#include "moving_average.h"

// x s/flash    * 20 runs/s  = x*20 runs/flash
#define FLASH_PERIOD (1 * 20)
// how many multiples of 50ms
#define DEBOUNCE_PERIOD 7

#define MOVING_AVERAGE_X moving_average_s

/* setup and initialize pins and peripherals */

/* end setup and initialize pins and peripherals */


typedef enum {
	TEMP_DISPLAY, 
	FLASHING
} Mode;

static volatile uint_fast16_t ticks;

void displayTemp(float temp) {
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

// returns 1 on mode change, 0 otherwise
int runMode() {
	
	// the initializations should be unecessary because of zero initialization
	static int countToBlink = 0;
	static Mode currentMode = TEMP_DISPLAY;
	static FilterState filterState = {0};
	static LED_state flashState = ON;
	
	switch(currentMode) {
		case TEMP_DISPLAY:
			// put temperature reading here
			//displayTemp(MOVING_AVERAGE_X(getTemp_celcius(), &filterState));
			displayTemp(getTemp_celcius());
			// send temperature data to debug port for analysis
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
		// this line is fast and loose with enums
		currentMode = currentMode? TEMP_DISPLAY : FLASHING;
		// set all lights to zero
		LED_setAll(OFF);
		// doesn't need to be set half of the time
		countToBlink = FLASH_PERIOD;
		return DEBOUNCE_PERIOD;
	} else
		return 0;
}

int main()
{
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
			debouncing = runMode();
	}	
}



