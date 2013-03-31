#include <stdio.h>

#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "TIM3.h"
#include "globals.h"

#include "slave.h"
#include "master.h"

#include "button.h"
#include "LED.h"
#include "acc.h"
#include "wirelessDriver.h"

#define APP_FREQ 100


void TIM3_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
	osSignalSet(mainThread_ID, 0x1);
}

/*!
 @brief Program entry point
 */
int main (void) {
	Mode currentMode;
	mainThread_ID = osThreadGetId();

	
	buttonInit();
	LED_init();
	acc_init();
	TIM3_Init(50);
	dma_init();
	


	
	
	// record operating mode
	currentMode = buttonIsPushed() ? SLAVE : MASTER;
	
	osSemaphoreDef(dmaSema);
	dmaSema_ID = osSemaphoreCreate(osSemaphore (dmaSema), 1);
	
	osSemaphoreDef(dmaComplete);
	dmaComplete_ID = osSemaphoreCreate(osSemaphore (dmaComplete), 1);
	
	// lock it. DMA will release
	osSemaphoreWait(dmaComplete_ID, osWaitForever);
	
	
	if(RF2500_Init() )
		while(1); //death pit

	
	// Assuming only assigned thread ID can receive from queue
	// slave wireless puts     slave main gets
	// master wireless gets    master main puts
	osMessageQDef ( queue, 1, uint32_t );
	
	// the slave_run and master_run functions
	// never return
	switch( currentMode) {
		case SLAVE: {
			osThreadDef (slave_wireless, osPriorityNormal, 1, 0);
			wirelessThread_ID = osThreadCreate( osThread (slave_wireless), NULL);
			
			queue_ID = osMessageCreate ( osMessageQ( queue ), mainThread_ID);
			
			TIM3_Init(50);
			slave_run();      
		} break;
		case MASTER: {
			osThreadDef (master_wireless, osPriorityNormal, 1, 0);
			wirelessThread_ID = osThreadCreate( osThread (master_wireless), NULL);
			
			queue_ID = osMessageCreate ( osMessageQ( queue ), wirelessThread_ID);
			
			TIM3_Init(50);
			master_run();
		} break;
	}
}
