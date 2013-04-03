#include "slave.h"

uint8_t write_buff[12] = {LIS302DL_OUT_X_ADDR | 0x40 | 0x80};
uint8_t read_buff[12];


void slave_run(void) {
	static FilterState fs[3];
	int8_t filteredAcc[3];
	
	int timeCount = 0;
	LED_state leds = OFF;
	
	
	while(1) {
		// get message from wireless
		uint8_t* msg;
		msg = osMessageGet(queue_ID, osWaitForever).value.p;
		
		// read from accelerometer
		SPI_DMA_simple(write_buff, read_buff, ACC, 1 + 3*2);
		
		for (int i = 0; i<3; i++) {
			filteredAcc[i] = moving_average(read_buff[1+2*i], &fs[i]);
		}
		

		
		
		
		int difference = 0;
		for(int i = 0; i<3; i++)
			difference += abs((int8_t)filteredAcc[i] - (int8_t)msg[i]);
		
		if(difference < THRESHOLD) {
			if(timeCount < FLASH_PERIOD)
				timeCount++;
			else {
				timeCount = 0;
				//leds = leds ? OFF : ON;
				//set on for testing
				leds = ON;
			}
		} else
			leds = OFF;

		LED_setAll(leds);
	}
}

void slave_wireless(const void* p) {
	uint8_t received[4];
	while(1) {
		// on TIM3 tick
		osSignalWait(0x1, osWaitForever);
		if(receiveAccelData(received))
			osMessagePut(queue_ID, (uint32_t)(&received[0]), osWaitForever);
	}
	
}
