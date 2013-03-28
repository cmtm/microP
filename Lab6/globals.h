#pragma once

#include "cmsis_os.h"

typedef enum {
	SLAVE, 
	MASTER
} Mode;

extern osThreadId mainThread_ID;
extern osThreadId wirelessThread_ID;

extern osMessageQId queue_ID;
extern osSemaphoreId dmaSema_ID;
