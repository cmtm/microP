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
	
	osSemaphoreDef(dmaSema);
	dmaSema_ID = osSemaphoreCreate(osSemaphore (dmaSema), 1);
	
	osSemaphoreDef(dmaComplete);
	dmaComplete_ID = osSemaphoreCreate(osSemaphore (dmaComplete), 1);
	
	buttonInit();
	LED_init();
	acc_init();
	TIM3_Init(50);
	dma_init();
	
	// record operating mode
	currentMode = buttonIsPushed() ? SLAVE : MASTER;

	
	// lock it. DMA will release
	osSemaphoreWait(dmaComplete_ID, osWaitForever);
	
	
	if(RF2500_Init() )
		while(1); //death pit

	static uint8_t read_buff[7];
	static uint8_t write_buff[7]= {LIS302DL_OUT_X_ADDR|0x40|0x80,0x00,0x00,0x00,0x00,0x00,0x00};

	
	SPI_DMA_xfer(write_buff, read_buff, LIS302DL_SPI_CS_GPIO_PORT, LIS302DL_SPI_CS_PIN, 7);				

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
			
			TIM3_Init(10);
			slave_run();      
		} break;
		case MASTER: {
			osThreadDef (master_wireless, osPriorityNormal, 1, 0);
			wirelessThread_ID = osThreadCreate( osThread (master_wireless), NULL);
			
			queue_ID = osMessageCreate ( osMessageQ( queue ), wirelessThread_ID);
			
			TIM3_Init(10);
			master_run();
		} break;
	}
}
