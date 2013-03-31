#pragma once

#include <stdint.h>
#include "dma.h"
#include "moving_average.h"
#include "globals.h"
#include "LED.h"
#include "wirelessDriver.h"


void master_run(void);

void master_wireless(const void*);
