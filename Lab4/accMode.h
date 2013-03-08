#pragma once

#include <math.h>
#include "acc.h"
#include "button.h"
#include "LED.h"
#include "moving_average.h"
#include "cmsis_os.h"

extern osSemaphoreId accSema;

void aM_init(void);

void aM_run(const void*);