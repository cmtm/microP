#pragma once

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <math.h>

#define WaveTableSize 1000
#define floatWaveTableSize 1000.0



typedef enum {
	SAWTOOTH,
	FM
} Synth;

void dspCreateWaveform (uint8_t volume, Synth currentSynth, uint8_t alpha, uint8_t beta, uint8_t note, uint16_t* target, uint32_t bufferSize);
float FMSynthesis(uint16_t note, uint8_t alpha, uint8_t beta,  int Fs);

float FreqToIncrement(float freq,  float Fs);

float LPFR(float in, float q, float fc);

float KeyToFreq(uint8_t note);

uint16_t FloatToAudio(float in);

float Distortion (float in);
void dspInit(void);
