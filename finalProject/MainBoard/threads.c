#include "threads.h"



osSemaphoreDef(printSema);


printSema_ID = osSemaphoreCreate(osSemaphore (printSema), 1);



void lcdThread();