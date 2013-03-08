#include "accMode.h"

void aM_init() {
	acc_init();
}

typedef enum {
	ANGLE, 
	MOTION
} Mode;

osSemaphoreDef(accSema);


static Mode currentMode = ANGLE;

osSemaphoreId accSema;
	
static void aM_setLEDs() {
	
	switch(currentMode) {
		case ANGLE:
		break;
		case MOTION:
		break;
	}
}

void aM_run(const void* args) {
	static int countToBlink = 0;
	static int debouncing = 0;
	static FilterState filterState = {0};

	
	switch(currentMode) {
		case ANGLE:
		break;
		case MOTION:
		break;
	}
	
	// only do this part (input/output) if this thread has control
	if( osSemaphoreWait(accSema, 0)) {
	
		if( !debouncing && buttonIsPushed() ) {
			debouncing = 1;
			currentMode = currentMode? ANGLE : MOTION;
		} else if ( debouncing && !buttonIsPushed() )
			debouncing = 0;
		
		aM_setLEDs();
		osSemaphoreRelease(accSema);
	}
}
