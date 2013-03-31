#include "globals.h"

osThreadId mainThread_ID;
osThreadId wirelessThread_ID;

osMessageQId queue_ID;

// guard accesses to DMA
osSemaphoreId dmaSema_ID;
// DMA complete
osSemaphoreId dmaComplete_ID;
