#include "arm_math.h"

#include "stm32f4xx.h"
#include "cmsis_os.h"

#include BUFFER_SIZE 512
#include MAX_VOLUME 10



osThreadDef(wirelessThread, osPriorityNormal, 1, 0);
osThreadDef(lcdThread, osPriorityNormal, 1, 0);


/*!
 @brief Program entry point
 */
int main (void) {
	// from 0 to MAX_VOLUME
	uint8_t volume = 5;
	
	Synth currentSynth = SAWTOOTH;
	
	uint16_t buffers[2][BUFFER_SIZE];
	uint8_t currentBuff = 1;
	
	
	// TODO: initialize everything

	// Start thread
	osThreadCreate(osThread(wirelessThread), NULL);
	osThreadCreate(osThread(lcdThread), NULL);
	
	// TODO: kickoff DMA
	
	while(1) {
		Key keyPushed;
		uint16_t note = 0;
		
		// wait for DMA
		osSignalWait(0x01, osWaitForever);
		
		keyPushed = keyGetPushed();
		switch(keyPushed) {
			case POUND:
				volume--;
			break
			case HASH:
				volume++;
			break;
			case k0:
				if(currentSynth == SAWTOOTH)
					currentSynth = FM;
				else
					currentSynth = SAWTOOTH;
			break;
			default:
				note = (uint16_t) keyPushed;
			// TODO: other cases?			
		}
		
		printToLCD(volume, currentSynth, wirelessGetPitch(), wirelessGetYaw(), note);
		
		createWaveform(volume, currentSynth, wirelessGetPitch(), wirelessGetYaw(), note, buffers[currentBuff], BUFFER_SIZE);
		currentBuff = currentBuff ? 0 : 1;
	}

}
