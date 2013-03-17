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
	
    //this will set the LEDs according to the state
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
    
    //every 50 milliseconds, this will happen
	static int countToBlink = 0;
	static int debouncing = 0;
	static FilterState filterState = {0};

	//we will look at the current mode
	switch(currentMode) {
        //by default it is in temperature display
		case TEMP_DISPLAY: {
            //printf("tempDISPLAY\n");
            //when in temperature mode, retrieve the temperature and display it
			temp = moving_average(getTemp_celcius(), &filterState);
		} break;
		case FLASHING:
           // printf("tempFLASHING\n");
            // if in flashing mode, set the period for on or off
			if(countToBlink != 0)
				countToBlink--;
			else {
				countToBlink = FLASH_PERIOD;
				flashState = flashState ? OFF : ON;
			
			}
		break;
	}
	
	// every loop, we check if the semaphore is available
    // let's see what happens in here
	if( osSemaphoreWait(tempSema, 0)) {
        printf("TEMP HAS THE SEMAPHORE\n");
        //debounding is originally set to zero. if the button is pushed during this period,
        //then we enter this state
		if( !debouncing && buttonIsPushed() ) {
            //debouncing is immediately set to 1 and we change the state to the opposite one
			debouncing = 1;
			currentMode = currentMode? TEMP_DISPLAY : FLASHING;
			countToBlink = FLASH_PERIOD;	
            //let's say debouncing is 1 after, but the button is still pushed. this is ignored
            //if the next function gets the semaphore, debouncing will be changed back next time only
            //so if it enters here and will become zero again, ready to test again    
		} else if ( debouncing && !buttonIsPushed() )
			debouncing = 0;
		
		tM_setLEDs();
		osSemaphoreRelease(tempSema);
	}
}


