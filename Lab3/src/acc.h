#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery_lis302dl.h"

#include <math.h>

// Initialize the LIS302DL accelerometer functionality


void acc_init(void);
void acc_calib(uint8_t *offset);
void acc_read(uint8_t *accData);
