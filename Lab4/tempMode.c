#include "tempMode.h"

/// runs/flash
#define FLASH_PERIOD 20

typedef enum {
	TEMP_DISPLAY, 
	FLASHING
} Mode;

osSemaphoreDef(tempSema);

osSemaphoreId tempSema;
osThreadId tempThread_ID;

void tM_init() {
	ADC_init();
	tempSema = osSemaphoreCreate(osSemaphore (tempSema), 1);
}

	
static void displayTemp(float temp) {

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

void tM_run(const void* args) {
    
    //every 50 milliseconds, this will happen
	int countToBlink = 0;
	FilterState filterState = {0};
	float temp;
	LED_state flashState = ON;
	Mode currentMode = TEMP_DISPLAY;

	while (1) {
		osSignalWait(0x1, osWaitForever);
		//we will look at the current mode
		switch(currentMode) {
			//by default it is in temperature display
			case TEMP_DISPLAY: {
				//printf("tempDISPLAY\n");
				//when in temperature mode, retrieve the temperature and display it
				temp = moving_average(getTemp_celcius(), &filterState);
				if( osSemaphoreWait(tempSema, 0)) {
					
					displayTemp(temp);
					if(buttonWasPushed())
						currentMode = FLASHING;
	
					osSemaphoreRelease(tempSema);
				}
	
			} break;
			case FLASHING: {
				if( osSemaphoreWait(tempSema, 0)) {
					
					LED_setAll(flashState);
					if(buttonWasPushed())
						currentMode = TEMP_DISPLAY;
	
					osSemaphoreRelease(tempSema);
				}
				
				if(countToBlink > 0)
					countToBlink--;
				else {
					countToBlink = FLASH_PERIOD;
					flashState = flashState ? OFF : ON;			
				}
				
			} break;
		}
	}
	
}


