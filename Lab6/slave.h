#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "dma.h"
#include "moving_average.h"
#include "globals.h"
#include "LED.h"

#define THRESHOLD 30
#define FLASH_PERIOD 20

void slave_run(void);

void slave_wireless(const void*);
