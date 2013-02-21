#include "acc.h"

/* Choose between scale of 2g-8g and can measure accelerations with output 
of 100Hz or 400Hz. The accelerometer has an I2C and SPI digital serial interface
for 3 acceleration outputs. This means that no external ADC is needed. */


/* Function to initialize the LIS302DL. */
void acc_init(void)
{
	LIS302DL_InitTypeDef LIS302DL_conf; //Create the initializing structure
	LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;  
	
	LIS302DL_conf.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;	// Set Low Power Mode
	LIS302DL_conf.Output_DataRate = LIS302DL_DATARATE_100;	//Set the output data rate to 100Hz
	LIS302DL_conf.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE |	LIS302DL_Z_ENABLE;	// Enable all axes
	LIS302DL_conf.Full_Scale = LIS302DL_FULLSCALE_2_3;	// 2g full scale
	LIS302DL_conf.Self_Test = LIS302DL_SELFTEST_NORMAL;	// ????
	
	LIS302DL_Init(&LIS302DL_conf); //Initialize the accelerometer with the parameters set above

  /* Set configuration of Internal High Pass Filter of LIS302DL*/
  //LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_NOTLATCHED;
  //LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE;
	//LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE;
  //LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);	
}

/*Takes a pointer to an array. Stores in the array the calibration values for the accelertometer*/
void acc_calib(uint8_t *offset)
{
	offset[1]=0;
	offset[2]=0;
	offset[3]=0;
}

/*Reads the LIS302DL output register. Stores the value in variables x, y and z created in main*/
void acc_read(uint8_t *accData) 
{
	  LIS302DL_Read(&accData[1], LIS302DL_OUT_X_ADDR, 8);
	  LIS302DL_Read(&accData[2], LIS302DL_OUT_Y_ADDR, 8);
	  LIS302DL_Read(&accData[3], LIS302DL_OUT_Z_ADDR, 8);
	
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

