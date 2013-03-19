#include "acc.h"

#define X_OFFSET -30
#define Y_OFFSET -14
#define Z_OFFSET +36

#define X_SCALE 985
#define Y_SCALE 986
#define Z_SCALE 1008

#define GLOBAL_SCALE 981

/* Choose between scale of 2g-8g and can measure accelerations with output 
of 100Hz or 400Hz. The accelerometer has an I2C and SPI digital serial interface
for 3 acceleration outputs. This means that no external ADC is needed. */

static void interruptEnable(void);

/* Function to initialize the LIS302DL. */
void acc_init(void) {
	LIS302DL_InitTypeDef LIS302DL_conf;
	LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;

	
	LIS302DL_conf.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	LIS302DL_conf.Output_DataRate = LIS302DL_DATARATE_100;
	LIS302DL_conf.Axes_Enable = LIS302DL_XYZ_ENABLE;
	LIS302DL_conf.Full_Scale = LIS302DL_FULLSCALE_2_3;
	LIS302DL_conf.Self_Test = LIS302DL_SELFTEST_NORMAL;
	LIS302DL_Init(&LIS302DL_conf);

	LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_LATCHED;
	LIS302DL_InterruptStruct.SingleClick_Axes=LIS302DL_CLICKINTERRUPT_Z_ENABLE;
	LIS302DL_InterruptStruct.DoubleClick_Axes=LIS302DL_DOUBLECLICKINTERRUPT_XYZ_DISABLE;
	LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);
	
	interruptEnable();
}

static void interruptEnable() {
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	uint8_t ctrl;
	
	//enable single click interrupt
	ctrl=0x7;
	LIS302DL_Write( &ctrl,LIS302DL_CTRL_REG3_ADDR, 1);
	
	//initialise the interrupt for values bigger than that threshold on Z threshold
	ctrl=0x04;
	LIS302DL_Write( &ctrl, LIS302DL_CLICK_THSZ_REG_ADDR, 1);
	
	//set the timing
	ctrl=0x01;
	LIS302DL_Write( &ctrl, LIS302DL_CLICK_TIMELIMIT_REG_ADDR, 1);
	
	ctrl = 0x00;
	LIS302DL_Write( &ctrl, LIS302DL_CLICK_LATENCY_REG_ADDR, 1);
	// ctrl = 0x00;
	// LIS302DL_Write( &ctrl, LIS302DL_CLICK_WINDOW_REG_ADDR, 1);	
	
	
	
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	/* Connect PE0 pin to EXTI Line0*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	acc_clearLatch();
}

void acc_clearLatch() {
	uint8_t junk[2];
	LIS302DL_Read( junk, LIS302DL_CLICK_SRC_REG_ADDR, 2);
}


/*Reads the LIS302DL output register. Stores the value in variables x, y and z created in main*/
void acc_read(int32_t* accData) {
	
	LIS302DL_ReadACC(accData);
	
	accData[0] += X_OFFSET;
	accData[1] += Y_OFFSET;
	accData[2] += Z_OFFSET;
	
	accData[0] *= GLOBAL_SCALE;
	accData[1] *= GLOBAL_SCALE;
	accData[2] *= GLOBAL_SCALE;
	
	accData[0] /= X_SCALE;
	accData[1] /= Y_SCALE;
	accData[2] /= Z_SCALE;
}

void acc_convertRaw(uint8_t* in, int16_t* out) {
    for(int i=0; i < 3; i++){
        *out =(int32_t)(LIS302DL_SENSITIVITY_2_3G *  (int8_t)in[2*i]);
        out++;
    }
	acc_calibrate(out);
}

void acc_calibrate(int16_t* accData) {
    
    
	accData[0] += X_OFFSET;
	accData[1] += Y_OFFSET;
	accData[2] += Z_OFFSET;
	
	accData[0] *= GLOBAL_SCALE;
	accData[1] *= GLOBAL_SCALE;
	accData[2] *= GLOBAL_SCALE;
	
	accData[0] /= X_SCALE;
	accData[1] /= Y_SCALE;
	accData[2] /= Z_SCALE;
}

