#include "accMode.h"

void dma_init() {
	
}

void aM_init() {
	acc_init();
	dma_init();
}

typedef enum {
	ANGLE, 
	MOTION
} Mode;

osSemaphoreDef(accSema);


static Mode currentMode = ANGLE;

volatile osSemaphoreId accSema;
	
static void aM_setLEDs() {
	
	switch(currentMode) {
		case ANGLE:
		break;
		case MOTION:
		break;
	}
}

void aM_run(const void* args) {
	static int debouncing = 0;

	
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
