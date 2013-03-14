#include "tempMode.h"

/// runs/flash
#define FLASH_PERIOD 20

typedef enum {
	TEMP_DISPLAY, 
	FLASHING
} Mode;

osSemaphoreDef(tempSema);


static Mode currentMode = TEMP_DISPLAY;
static LED_state flashState = ON;
static float temp = 0.0;

volatile osSemaphoreId tempSema;

void tM_init() {
	ADC_init();
	tempSema = osSemaphoreCreate(osSemaphore (tempSema), 1);
}
	
static void tM_setLEDs() {
	
	switch(currentMode) {
		case TEMP_DISPLAY: {

			float normalized = fmod(temp - 2, 8);
			
			if(normalized < 2)
				LED_set(ON, OFF, OFF, OFF);
			else if (normalized < 4)
				LED_set(ON, ON, OFF, OFF);
			else if (normalized < 6)
				LED_set(ON, ON, ON, OFF);
			else
				LED_set(ON, ON, ON, ON);
		} break;
		case FLASHING:
			LED_setAll(flashState);
		break;
	}
}

void tM_run(const void* args) {
	static int countToBlink = 0;
	static int debouncing = 0;
	static FilterState filterState = {0};

	
	switch(currentMode) {
		case TEMP_DISPLAY: {
			temp = moving_average(getTemp_celcius(), &filterState);
		} break;
		case FLASHING:
			if(countToBlink != 0)
				countToBlink--;
			else {
				countToBlink = FLASH_PERIOD;
				flashState = flashState ? OFF : ON;
			
			}
		break;
	}
	
	// only do this part (input/output) if this thread has control
	if( osSemaphoreWait(tempSema, 0)) {
	
		if( !debouncing && buttonIsPushed() ) {
			debouncing = 1;
			currentMode = currentMode? TEMP_DISPLAY : FLASHING;
			countToBlink = FLASH_PERIOD;	
		} else if ( debouncing && !buttonIsPushed() )
			debouncing = 0;
		
		tM_setLEDs();
		osSemaphoreRelease(tempSema);
	}
}


