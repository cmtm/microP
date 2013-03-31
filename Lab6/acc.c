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

//static void interruptEnable(void);

/* Function to initialize the LIS302DL. */
void acc_init(void) {
	
	LIS302DL_InitTypeDef LIS302DL_conf;

	
	LIS302DL_conf.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	LIS302DL_conf.Output_DataRate = LIS302DL_DATARATE_100;
	LIS302DL_conf.Axes_Enable = LIS302DL_XYZ_ENABLE;
	LIS302DL_conf.Full_Scale = LIS302DL_FULLSCALE_2_3;
	LIS302DL_conf.Self_Test = LIS302DL_SELFTEST_NORMAL;
	LIS302DL_Init(&LIS302DL_conf);
    
}

void acc_clearLatch() {
	uint8_t junk[2];
	LIS302DL_Read( junk, LIS302DL_CLICK_SRC_REG_ADDR, 2);
}


/*Reads the LIS302DL output register. Stores the value in variables x, y and z created in main*/
void acc_read(int32_t* accData) {
	
	LIS302DL_ReadACC(accData);
	
	acc_calibrate(accData);
}

void acc_convertRaw(uint8_t* in, int32_t* out) {
    for(int i=0; i < 3; i++){
        *out =(int32_t)(LIS302DL_SENSITIVITY_2_3G *  (int8_t)in[2*i]);
        out++;
    }
	acc_calibrate(out);
}

void acc_calibrate(int32_t* accData) {
    
    
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

