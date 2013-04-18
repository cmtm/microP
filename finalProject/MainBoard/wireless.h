#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "wirelessDriver.h"
#include "dma.h"
#include "TIM3.h"

void wirelessThread(const void* args);

int wirelessInit(void);

uint8_t wirelessGetAlpha(void);

uint8_t wirelessGetBeta(void);
