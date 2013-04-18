#include "wireless.h"

static uint8_t alpha;
static uint8_t beta;

osSemaphoreDef(anglesSema);
osSemaphoreId anglesSema_ID;

osThreadId wireless_ID;

void TIM3_IRQHandler(void) {
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
	osSignalSet(wireless_ID, 0x01);
}

int wirelessInit() {
	wireless_ID = osThreadGetId();
	dma_init();
	TIM3_Init(20);
	return RF2500_Init();	
}



void wirelessThread(const void* args) {
	if(!wirelessInit())
		while(1);
	anglesSema_ID = osSemaphoreCreate(osSemaphore (anglesSema), 1);
	uint8_t received[4];
	while(1) {
		// on TIM3 tick
		osSignalWait(0x1, osWaitForever);
		if(receiveAccelData(received)) {
			osSemaphoreWait(anglesSema_ID, osWaitForever);
			alpha = received[0];
			beta = received[1];
			osSemaphoreRelease(anglesSema_ID);
		}	
	}
}

uint8_t wirelessGetAlpha() {
	uint8_t buff;
	osSemaphoreWait(anglesSema_ID, osWaitForever);
	buff = alpha;
	osSemaphoreRelease(anglesSema_ID);
	return buff;
}

uint8_t wirelessGetBeta() {
	uint8_t buff;
	osSemaphoreWait(anglesSema_ID, osWaitForever);
	buff = beta;
	osSemaphoreRelease(anglesSema_ID);
	return buff;
}
