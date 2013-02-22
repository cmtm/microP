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


/* Function to initialize the LIS302DL. */
void acc_init(void) {
	uint8_t ctrl = 0;
	LIS302DL_InitTypeDef LIS302DL_conf; //Create the initializing structure
	LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	LIS302DL_conf.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;	// Set Low Power Mode
	LIS302DL_conf.Output_DataRate = LIS302DL_DATARATE_100;	//Set the output data rate to 100Hz
	LIS302DL_conf.Axes_Enable = LIS302DL_XYZ_ENABLE;	// Enable all axes
	LIS302DL_conf.Full_Scale = LIS302DL_FULLSCALE_2_3;	// 2g full scale
	LIS302DL_conf.Self_Test = LIS302DL_SELFTEST_NORMAL;	// ????
	
	LIS302DL_Init(&LIS302DL_conf); //Initialize the accelerometer with the parameters set above

  LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_NOTLATCHED;
  LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE;
	// LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE;
  LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);
	
	for(int i = 0; i < 1000; i++)
	
	 /* Configure Interrupt control register: enable Click interrupt1 */
  ctrl = 0x07;
  LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG3_ADDR, 1);
  
  /* Enable Interrupt generation on click/double click on Z axis */
  ctrl = 0x70;
  LIS302DL_Write(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);
  
  /* Configure Click Threshold on X/Y axis (10 x 0.5g) */
  ctrl = 0xAA;
  LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSY_X_REG_ADDR, 1);
  
  /* Configure Click Threshold on Z axis (10 x 0.5g) */
  ctrl = 0x0A;
  LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSZ_REG_ADDR, 1);
  
  /* Configure Time Limit */
  ctrl = 0x03;
  LIS302DL_Write(&ctrl, LIS302DL_CLICK_TIMELIMIT_REG_ADDR, 1);
    
  /* Configure Latency */
  ctrl = 0x7F;
  LIS302DL_Write(&ctrl, LIS302DL_CLICK_LATENCY_REG_ADDR, 1);
  
  /* Configure Click Window */
  ctrl = 0x7F;
  LIS302DL_Write(&ctrl, LIS302DL_CLICK_WINDOW_REG_ADDR, 1);
	
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
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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
/*Measuring the tilt angle from the acceleration*/
void calculate_angle (uint8_t *accData, double *angles) {

	/*Change the type of the data from uint8_t to double*/
	double accX = (double) (accData[0]);
	double accY = (double) (accData[1]);
	double accZ = (double) (accData[2]);

	//angles[0] = arctan(accX/sqrt(accY*accY+accZ*accZ));	//Yaw angle
	//angles[1] = arctan(Acc_y, Acc_z);	//Roll angle
	//angles[2] = arctan(Acc_x, Acc_z);	//Pitch angle	
}

