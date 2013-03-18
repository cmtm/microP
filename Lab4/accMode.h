#pragma once

#include <stdio.h>
#include <math.h>
#include "acc.h"
#include "button.h"
#include "LED.h"
#include "moving_average.h"
#include "cmsis_os.h"
#include "dma.h"

extern osSemaphoreId accSema;
extern osThreadId accThread_ID; 

void aM_init(void);

void aM_run(const void*);
