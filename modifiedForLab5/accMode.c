#include "accMode.h"

#define BLOCK_SIZE 10
#define NUM_COEFFS 19

float32_t coeffs[] = {
0.07416114002096, 0.03270293265285, 0.03863816947328,
0.04444276039114, 0.04979857683070, 0.05449897183339,
0.05841301164775, 0.06134989872289, 0.06313835392955,
0.06373119762273, 0.06313835392955, 0.06134989872289,
0.05841301164775, 0.05449897183339, 0.04979857683070,
0.04444276039114, 0.03863816947328, 0.03270293265285,
0.07416114002096};

osThreadId accThread_ID;
uint8_t send[7] = {LIS302DL_OUT_X_ADDR | 0x40 | 0x80,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t receive[7];

    
osSemaphoreDef(accSema);
osSemaphoreId accSema;

 void DMA2_Stream0_IRQHandler(void) {
     
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0)) {
		
		/* Set CS high */
		GPIO_SetBits(LIS302DL_SPI_CS_GPIO_PORT, LIS302DL_SPI_CS_PIN);
		
		/* Clear all the flags */
		DMA_ClearFlag(DMA2_Stream0, DMA_FLAG_TCIF0);
		DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3);
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		
		/* Disable DMA */
		DMA2_Stream0->CR |= 0;
		DMA2_Stream3->CR |= 0;
		
		osSignalSet(accThread_ID, 0x2);
	} 
}

void aM_init() {
	acc_init();
    dma_init();
    accSema = osSemaphoreCreate(osSemaphore (accSema), 1);	
}

void displayTiltDirection(int16_t* accData) {
	/*Change the type of the data from uint32_t to double*/
	double accX = (double) (accData[0]);
	double accY = (double) (accData[1]);
	double accZ = (double) (accData[2]);
	
	double roll;
	double pitch;


	roll = atan2(accX, sqrt(accY*accY + accZ*accZ)) * 180 / PI;	//Roll angle
	pitch = atan2(accY, sqrt(accX*accX + accZ*accZ)) * 180 / PI;	//Pitch angle	
	
	if (accZ<0) {
		if (accX>0)	roll = 180 - roll;
		else roll = -roll - 180;
		
		if (accY>0)	pitch = 180 - pitch;
		else pitch = -pitch - 180;		
	}
	
	LED_state leds[4] = {OFF, OFF, OFF, OFF};
	
	if (roll < -10)
		leds[1] = ON;
	else if (roll > 10)
		leds[3] = ON;
	
	if (pitch < -10)
		leds[0] = ON;
	else if (pitch > 10)
		leds[2] = ON;
	
	LED_set_ar(leds);
	
}

void displayMotionDirection(int16_t* deltaAcc) {
	LED_state leds[4] = {OFF, OFF, OFF, OFF};
	
	if (deltaAcc[1] > 100)
		leds[0] = ON;
	else if (deltaAcc[1] < -100)
		leds[2] = ON;
	
	if (deltaAcc[0] > 100)
		leds[1] = ON;
	else if (deltaAcc[0] < -100)
		leds[3] = ON;
	
	LED_set_ar(leds);
}

void printAngles(int16_t* accData) {

	/*Change the type of the data from uint32_t to double*/
	double accX = (double) (accData[0]);
	double accY = (double) (accData[1]);
	double accZ = (double) (accData[2]);
	
	double roll;
	double pitch;


	roll = atan2(accX, sqrt(accY*accY + accZ*accZ)) * 180 / PI;	//Roll angle
	pitch = atan2(accY, sqrt(accX*accX + accZ*accZ)) * 180 / PI;	//Pitch angle	
	
	if (accZ<0) {
		if (accX>0)	roll = 180 - roll;
		else roll = -roll - 180;
		
		if (accY>0)	pitch = 180 - pitch;
		else pitch = -pitch - 180;
		
	}	
	printf("Roll: %f, Pitch: %f\n", roll, pitch);
}

typedef enum {
	ANGLE, 
	MOTION
} Mode;

//copy pasted for now
void acc_read_DMA(void) {
	/* Set the CS_GPIO_PORT low */
	GPIO_ResetBits(LIS302DL_SPI_CS_GPIO_PORT, LIS302DL_SPI_CS_PIN);
	
	
	DMA2_Stream0->M0AR = (uint32_t)(&receive[0]); /* address of received array */
	DMA2_Stream3->M0AR = (uint32_t)(&send[0]);  
	
	
	DMA2_Stream0->NDTR = 7;                                      /* Expect to receive 7 bytes */
	/* transmit part -- enable */
	DMA2_Stream3->NDTR = 7;                                      /* Expect to transmit 7 bytes */
	/* address of sent array */
	
	
	/* Enable DMA for transmission and receiving */
	DMA2_Stream0->CR |= DMA_SxCR_EN;   /* Enable Rx */
	DMA2_Stream3->CR |= DMA_SxCR_EN;   /* Enable Tx */
}




void aM_run(const void* args) {
	
	Mode currentMode = ANGLE;
	// static FilterState fs[3] = {0,0,0};


	int16_t accData[3];
	int16_t filteredAccData[3] = {0, 0, 0};

	static int16_t blockIndex = 0;
	static int16_t block[BLOCK_SIZE][3];
	static q15_t coeffsQ15[NUM_COEFFS];
	static q15_t stateArray[NUM_COEFFS + BLOCK_SIZE - 1][3];

	static arm_fir_decimate_instance_q15 decimateState[3];
	
	arm_float_to_q15(coeffs, coeffsQ15, NUM_COEFFS);
	
	for (int i=0; i<3; i++)
		arm_fir_decimate_init_q15(&decimateState[i], NUM_COEFFS, 10, coeffsQ15, &stateArray[0][i], BLOCK_SIZE);   
	
	while(1) {
		osSignalWait(0x1, osWaitForever);
		
		acc_read_DMA();
		osSignalWait(0x2, osWaitForever);
		acc_convertRaw(&receive[1], &accData[0]);
		acc_calibrate(&accData[0]);
		
		for (int i=0; i<3; i++)
			block[blockIndex][i] = accData[i];

		blockIndex++;
			
		if( blockIndex == BLOCK_SIZE) {
			for(int i=0; i<3; i++)
				arm_fir_decimate_q15(&decimateState[i], &block[0][i], &filteredAccData[i], BLOCK_SIZE);
			blockIndex = 0;
		}

		//accData[0] = moving_average(accData[0], &filterState1);
		//accData[1] = moving_average(accData[1], &filterState2);
		//accData[2] = moving_average(accData[2], &filterState3);
		
		//printAngles(filteredAccData);
	
		switch(currentMode) {
			case ANGLE:
				if( osSemaphoreWait(accSema, 0)) {
					displayTiltDirection(filteredAccData);
					if( buttonWasPushed() )
						currentMode = MOTION;
					osSemaphoreRelease(accSema);
				}        
			break;
			case MOTION: {
				int16_t deltaAcc[3];
				for( int i = 0; i < 3; i++)
					deltaAcc[i] = accData[i] - filteredAccData[i];
				
				if( osSemaphoreWait(accSema, 0)) {
					displayMotionDirection(deltaAcc);
					if( buttonWasPushed() )
						currentMode = ANGLE;
					osSemaphoreRelease(accSema);
				} 
			} break;
		}
	}
}
