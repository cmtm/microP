#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "LED.h"
#include "button.h"
#include "accMode.h"
#include "tempMode.h"

volatile osThreadId mainThread;

typedef enum {
	TEMP_MODE, 
	ACC_MODE
} Mode;

void EXTI0_IRQHandler(void) {
	EXTI_ClearFlag(LIS302DL_SPI_INT1_EXTI_LINE);
	osSignalSet(mainThread, 0x1);
}

/*!
 @brief Program entry point
 */
int main (void) {
	Mode currentMode;
	mainThread = osThreadGetId();
	
	LED_init();
	buttonInit();
	tM_init();
	aM_init();
	
	// lock both mode semaphores, they are both in "used" mode
	osSemaphoreWait( tempSema, osWaitForever);
	osSemaphoreWait( accSema, osWaitForever);
	
	// start threads
	osTimerDef (tempTimer, tM_run);
	osTimerId tempTimer = osTimerCreate( osTimer (tempTimer), osTimerPeriodic, NULL);
	osTimerDef (accTimer, aM_run);
	osTimerId accTimer = osTimerCreate( osTimer (accTimer), osTimerPeriodic, NULL);
	
	osTimerStart(tempTimer, 50);
	osTimerStart(accTimer, 50);
	
	// start in TEMP_MODE
	currentMode = TEMP_MODE;
	osSemaphoreRelease( tempSema);
	
	
	while(1) {
        //will start and wait for a tap before anything happends
        //it will run the threads though
		osSignalWait(0x1, osWaitForever);
        
        // once we're past here, it means a tap has been detected!
        
		switch( currentMode) {
			case TEMP_MODE:
                // we wait until the semaphore becomes available
				osSemaphoreWait( tempSema, osWaitForever);
				acc_clearLatch();
				osSemaphoreRelease(accSema);
				currentMode = ACC_MODE;
                //when tapping occurs, show that it did because now the mode is something else
                printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n");            
			break;
			case ACC_MODE:                
				osSemaphoreWait( accSema, osWaitForever);
				acc_clearLatch();
				osSemaphoreRelease(tempSema);
				currentMode = TEMP_MODE;
              printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
			break;
		}		
	}
}
