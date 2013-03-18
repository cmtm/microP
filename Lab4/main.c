#include <stdio.h>

#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "TIM3.h"

#include "accMode.h"
#include "tempMode.h"

volatile osThreadId mainThread_ID;

typedef enum {
	TEMP_MODE, 
	ACC_MODE
} Mode;

void EXTI0_IRQHandler(void) {
	EXTI_ClearFlag(LIS302DL_SPI_INT1_EXTI_LINE);
	osSignalSet(mainThread_ID, 0x1);
}

void TIM3_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	osSignalSet(accThread_ID, 0x1);
	osSignalSet(tempThread_ID, 0x1);	
}

/*!
 @brief Program entry point
 */
int main (void) {
	Mode currentMode;
	mainThread_ID = osThreadGetId();
	
	// start threads. Low priority ensures main will finish
	osThreadDef (tM_run, osPriorityBelowNormal, 1, 0);
	tempThread_ID = osThreadCreate( osThread (tM_run), NULL);
	osThreadDef (aM_run, osPriorityBelowNormal, 1, 0);
	accThread_ID = osThreadCreate( osThread (aM_run), NULL);
	
	LED_init();
	buttonInit();
	tM_init();
	aM_init();
	
	// lock both mode semaphores, they are both in "used" mode
	osSemaphoreWait( tempSema, osWaitForever);
	osSemaphoreWait( accSema, osWaitForever);
	

	
	// start in TEMP_MODE
	currentMode = TEMP_MODE;
	osSemaphoreRelease( tempSema);
	
	TIM3_Init(20);
	
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
			break;
			case ACC_MODE:                
				osSemaphoreWait( accSema, osWaitForever);
				acc_clearLatch();
				osSemaphoreRelease(tempSema);
				currentMode = TEMP_MODE;
			break;
		}		
	}
}
