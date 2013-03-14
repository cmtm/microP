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
	
	// lock both mode semaphores
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
		osSignalWait(0x1, osWaitForever);
		
		switch( currentMode) {
			case TEMP_MODE:
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

void thread (void const *argument) {
	while(1){
		osDelay(1000);
		GPIOD->BSRRL = GPIO_Pin_12;
		osDelay(1000);
		GPIOD->BSRRH = GPIO_Pin_12;
	}
}
