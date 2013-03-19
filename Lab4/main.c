#include <stdio.h>

#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "TIM3.h"

#include "accMode.h"
#include "tempMode.h"

#define ACC_FREQ 100
#define TEMP_FREQ 20

#define FREQ_RATIO (ACC_FREQ / TEMP_FREQ)

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
	
	static int count = FREQ_RATIO;
	if (count <= 1) {
		count = FREQ_RATIO;
		osSignalSet(tempThread_ID, 0x1);
	} else
		count--;	
	
	osSignalSet(accThread_ID, 0x1);
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
	
	TIM3_Init(ACC_FREQ);
	
	while(1) {
        //will start and wait for a tap before anything happends
        //it will run the threads though
		osSignalWait(0x1, osWaitForever);
        
        // once we're past here, it means a tap has been detected!
        
		switch( currentMode) {
			case TEMP_MODE:
                // we wait until the semaphore becomes available
				osSemaphoreWait( tempSema, osWaitForever);
				osDelay	(500);
				acc_clearLatch();
				osSemaphoreRelease(accSema);
				currentMode = ACC_MODE;           
			break;
			case ACC_MODE:                
				osSemaphoreWait( accSema, osWaitForever);
				osDelay	(500);
				acc_clearLatch();
				osSemaphoreRelease(tempSema);
				currentMode = TEMP_MODE;
			break;
		}		
	}
}
