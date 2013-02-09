#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

typedef enum {
	ON,
	OFF
} LED_state;

void init_GPIO(void);

void LED_set(LED_state s0, LED_state s1, LED_state s2, LED_state s3);
void LED_setAll(LED_state s);
