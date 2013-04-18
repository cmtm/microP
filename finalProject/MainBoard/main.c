#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "dsp.h"
#include "keypad.h"
#include "lcd.h"
#include "led.h"
#include "wireless.h"
#include "audioDriver.h"
#include "stm32f4_discovery_lis302dl.h"

#define BUFFER_SIZE 1024
#define MAX_VOLUME 255




osThreadDef(wirelessThread, osPriorityNormal, 1, 0);
osThreadDef(lcdThread, osPriorityNormal, 1, 0);

LED_state toggle(LED_state s) {
	return s ? OFF : ON;
}

/*!
 @brief Program entry point
 */
int main (void) {
	// from 0 to MAX_VOLUME
	uint8_t volume = 5;
	LED_state toggler = OFF;
	
	Synth currentSynth = SAWTOOTH;
	
	uint16_t buffers[2][BUFFER_SIZE];
	uint8_t currentBuff = 1;
	
	
	// TODO: initialize everything
	// LIS302DL_LowLevel_Init();

	keyInit();
	dspInit();
	LED_init();
	lcdInit();
	Audio_MAL_Init((uint32_t)buffers[0], (uint32_t)buffers[1], BUFFER_SIZE, osThreadGetId());
	
	// TODO: remove this
	uint8_t ramp = 0;
		

	// Start thread
	//osThreadCreate(osThread(wirelessThread), NULL);
	osThreadId lcd_ID = osThreadCreate(osThread(lcdThread), NULL);
	
	
	while(1) {
		Key keyPushed;
		uint16_t note = 0;
		ramp += 3;
		ramp = ramp%250;
		
		// wait for DMA
		osSignalWait(0x01, osWaitForever);
		// TODO: debounce!
		keyPushed = keyGetPushed();
		switch(keyPushed) {
			case POUND:
				if(volume > 0) 
					volume--;
			break;
			case STAR:
				if(volume < MAX_VOLUME)
					volume++;
			break;
			case K0:
				if(currentSynth == SAWTOOTH)
					currentSynth = FM;
				else
					currentSynth = SAWTOOTH;
			break;
			default:
				note = (uint16_t) keyPushed;
		}
		
		LED_set(currentBuff ? toggler : OFF,              !currentBuff ? toggler : OFF,
			    currentBuff ? toggle(toggler) : OFF,      !currentBuff ? toggle(toggler) : OFF);
		toggler = toggle(toggler);
		lcdPrint(volume, currentSynth, wirelessGetAlpha(), wirelessGetBeta(), note);
		osSignalSet(lcd_ID, 0x1);
		
		//dspCreateWaveform(volume, currentSynth, wirelessGetAlpha(), wirelessGetBeta(), note, buffers[currentBuff], BUFFER_SIZE);
		dspCreateWaveform(volume, currentSynth, ramp, ramp, note, buffers[currentBuff], BUFFER_SIZE);

		currentBuff = currentBuff ? 0 : 1;
	}

}
