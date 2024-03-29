#pragma once 

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery_lis302dl.h"

// Initialize the LIS302DL accelerometer functionality


void acc_init(void);
void acc_read(int32_t* accData);
void acc_clearLatch(void);
void acc_calibrate(int32_t* accData);
void acc_convertRaw(uint8_t* in, int32_t* out);
