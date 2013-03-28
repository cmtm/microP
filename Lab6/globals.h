#pragma once

#include "cmsis_os.h"

typedef enum {
	SLAVE, 
	MASTER
} Mode;

typedef union {
    struct {
		int8_t x;
		int8_t y;
		int8_t z;
	} coordinate;
	int8_t array[3];
    uint32_t all;
} AccPack;




extern osThreadId mainThread_ID;
extern osThreadId wirelessThread_ID;

extern osMessageQId queue_ID;
extern osSemaphoreId dmaSema_ID;
