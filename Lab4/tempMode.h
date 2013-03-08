#pragma once

#include <math.h>
#include "ADC.h"
#include "button.h"
#include "LED.h"
#include "moving_average.h"
#include "cmsis_os.h"

extern osSemaphoreId tempSema;

void tM_init(void);

void tM_run(const void*);
