#include "slave.h"


void slave_run(void) {
	static FilterState fs[3];
	int8_t filteredAcc[3];
	
	int timeCount = 0;
	LED_state leds = OFF;
	
	uint8_t write_buff[12];
	uint8_t read_buff[12];
	
	while(1) {
		// on TIM3 tick
		osSignalWait(0x1, osWaitForever);
		// read from accelerometer
		SPI_DMA_simple(write_buff, read_buff, 1 + 3*2, ACC);
		
		for (int i = 0; i<3; i++) {
			filteredAcc[i] = moving_average(read_buff[1+2*i], &fs[i]);
		}
		
		// get message from wireless
		AccPack msg;		
		msg.all = osMessageGet(queue_ID, osWaitForever).value.v;
		
		
		
		int difference = 0;
		for(int i = 0; i<3; i++)
			difference += abs(filteredAcc[i] - msg.array[i]);
		
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
	//while(1) {
		// TODO wait for wireless data
		
		//osMessagePut(queue_ID, ap.all, osWaitForever);
	//}
	
}
