#pragma once

#include <stdio.h>
#include <math.h>

// #include "arm_fir_decimate_init_q15.h"
// #include "arm_fir_decimate_q15.h"
// #include "arm_float_to_q15.h"

//#define __CC_ARM 1
#include "arm_math.h"


#include "acc.h"
#include "button.h"
#include "LED.h"
#include "moving_average.h"
#include "cmsis_os.h"
#include "dma.h"
#include "decimate.h"

extern osSemaphoreId accSema;
extern osThreadId accThread_ID; 

void aM_init(void);

void aM_run(const void*);
