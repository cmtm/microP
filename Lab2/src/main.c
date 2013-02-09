#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "LED.h"
#include "ADC.h"
#include "button.h"
#include "moving_average.h"

// x s/flash    * 20 runs/s  = x*20 runs/flash
#define FLASH_PERIOD (1 * 20)

#define MOVING_AVERAGE_X moving_average_c

/* setup and initialize pins and peripherals */

/* end setup and initialize pins and peripherals */


typedef enum {
	TEMP_DISPLAY, 
	FLASHING
} Mode;

static volatile uint_fast16_t ticks;

void displayTemp(float temp_celcius) {
	// fill this
}

// INSERT SYSTICK HANDLER

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
	
	if(isButtonPushed()) {
		// this line is fast and loose with enums
		currentMode = currentMode? TEMP_DISPLAY : FLASHING;
		// doesn't need to be set half of the time
		countToBlink = FLASH_PERIOD;
		return 1;
	}
	
	return 0;
}

int main()
{
	int debouncing = 0;

	ticks = 0;

	// call init function(s)
	

	
	while(1) {
		// wait for interupt
		while(!ticks);	
		
		ticks--;		
		
		if(debouncing)
			debouncing = 0;
		else
			debouncing = runMode();	
	}	
}



