#include "accMode.h"

#define PI 3.14159265

volatile osThreadId accThread;
	uint8_t send[7] = {LIS302DL_OUT_X_ADDR | 0x40 | 0x80,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t receive[7];
    int32_t accData[3];
    
osSemaphoreDef(accSema);

volatile osSemaphoreId accSema;

void aM_init() {
	acc_init();
    dma_init();
    accSema = osSemaphoreCreate(osSemaphore (accSema), 1);
}

void printAngles(int32_t* accData) {

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
         
        osSignalSet(accThread, 0x1);
    } 
}

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
  

static Mode currentMode = ANGLE;
	
static void aM_setLEDs() {
	
	switch(currentMode) {
		case ANGLE:
		break;
		case MOTION:
		break;
	}
}

void aM_run(const void* args) { 
    accThread = osThreadGetId();
	static int debouncing = 0;
        
    static FilterState filterState1 = {0}, filterState2 = {0}, filterState3 = {0};
	
	switch(currentMode) {
		case ANGLE:
            //printf("Pitch: %d\n",accData[0]);
            printf("ANGLE MODE\n");
            acc_read_DMA();
            osSignalWait(0x1, osWaitForever);
            acc_convert(&receive[1], &accData[0]);
            acc_calibrate(&accData[0]);
        
        	//accData[0] = moving_average_c(accData[0], &filterState1);
            //accData[1] = moving_average_c(accData[1], &filterState2);
            //accData[2] = moving_average_c(accData[2], &filterState3);
        
           	printAngles(accData);
        
		break;
		case MOTION:
           printf("MOTION MODE\n");
		break;
	}
	
	// only do this part (input/output) if this thread has control
	if( osSemaphoreWait(accSema, 0)) {
         printf("ACC HAS THE SEMAPHORE\n");
		if( !debouncing && buttonIsPushed() ) {
			debouncing = 1;
			currentMode = currentMode? ANGLE : MOTION;
		} else if ( debouncing && !buttonIsPushed() )
			debouncing = 0;
		
		aM_setLEDs();
		osSemaphoreRelease(accSema);
	}
}
