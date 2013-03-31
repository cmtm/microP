#include "master.h"

void master_run(void) {
	static FilterState fs[3];
	int8_t filteredAcc[3];
	
	uint8_t write_buff[12];
	uint8_t read_buff[12];
	
	while(1) {
		// on TIM3 tick
		osSignalWait(0x1, osWaitForever);
		// read from accelerometer
		SPI_DMA_simple(write_buff, read_buff, ACC, 1 + 3*2);
		
		for (int i = 0; i<3; i++) {
			filteredAcc[i] = moving_average((int8_t)read_buff[1+2*i], &fs[i]);
		}
		
		AccPack ap = {filteredAcc[0], filteredAcc[1], filteredAcc[2]};
		
		osMessagePut(queue_ID, ap.all, osWaitForever);		
	}
}

void master_wireless(const void* p) {
	osEvent msg;

	while(1) {
		// wait for message from mainThread
		msg = osMessageGet(queue_ID, osWaitForever);
	
		// TODO deliver it to other board
	}
}
