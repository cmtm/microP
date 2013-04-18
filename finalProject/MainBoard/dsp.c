#include "dsp.h"


float SineWave[WaveTableSize];

float SawWave[WaveTableSize];


void dspInit(){

	int i;
	
	for (i = 0; i < WaveTableSize; i ++){
		SineWave[i] = sin(i*2*3.14159/floatWaveTableSize);
		SawWave[i] = (WaveTableSize - 2*i)/floatWaveTableSize;
	}
	
}

void dspCreateWaveform (uint8_t volume, Synth currentSynth, uint8_t alpha, uint8_t beta, uint8_t note, uint16_t* target, uint32_t bufferSize){
	static const int Fs = 48000;
	// Saw tooth parameters
	static float sawIndex = 0;
	static float SawFreq;
	static float fc;
	static float q;
	
	static float gain;
	
	float out;
	
	gain = (float) volume / (float) 0xFF;
	
	int i;
	
	if(0 == note){
		for(i=0;i<bufferSize;i++){
			target[i] = 0;
		}
		return;
	}
	
	if(SAWTOOTH == currentSynth){
	
		SawFreq = KeyToFreq(note); //freq = 440;
		
		q = (float) beta / (float)0xFF * 1.;
		
		fc = (float)alpha / (float)0xFF * 8000.0 + 20.; // f is from 20Hz to 8 kHz
		
		for(i = 0; i< bufferSize; i++){
			//out = gain * LPFR( SawWave[(int) sawIndex], fc, q);
			out = gain * SawWave[(int) sawIndex];
			out = Distortion(3.*out);
			
			target[i] = FloatToAudio ( out );
			
			sawIndex += FreqToIncrement(SawFreq,Fs);
			if(sawIndex >= WaveTableSize) sawIndex -= WaveTableSize;
			
		}
		
	}else if(FM == currentSynth)
	{
		
		for (i=0; i<bufferSize; i++){
			target[i] = FloatToAudio ( gain * FMSynthesis(note, alpha, beta, Fs));
		}
		
	}
	
}

uint16_t FloatToAudio(float in){
	
	if(in < -1) in = -1;
	if(in > 1) in = 1;
	
	return (uint16_t) ( ((in + 1.)/2.)*0x0FFF ); //-1 to 1 maps to 0 to 0x0x00FF
	
}

float FMSynthesis(uint16_t note, uint8_t alpha, uint8_t beta, int Fs){
			
			static float fc, fm, d, f;
			static float Im = 0;
			static float I = 0;
			float out;
			float f0;
			int N1, N2;
			
			N1 = ((float)alpha / (float)0xFF ) * 10. ;
			N2 = 1;
			// c/m  = n1/n2
	
			d = ((float) beta / (float) 0xFF ) * 400;
	
			f0 = KeyToFreq(note);
			fc = f0 * N1;
			fm = f0 * N2;	
			
			//fc = 800; //carrier frequency
			//fm = 4*fc;
			
			
			//f = fc + d * sin(2pifmt);
			f = fc + d*SineWave[(int)Im];
			Im += FreqToIncrement(fm, Fs);
			if(Im >= WaveTableSize) Im -= WaveTableSize;
			//printf("modulator Im = %f, f = %f \n", Im, f);
			
			I += FreqToIncrement(f, Fs);
			if(I >= WaveTableSize) I -= WaveTableSize;
			
			out = SineWave[(int)I];
			//printf("carrier I = %f, out = %f \n", I, out);
			
			return out;
}

float FreqToIncrement(float freq,  float Fs){
	return freq*floatWaveTableSize/Fs;
}

float LPFR(float in, float q, float fc){
	static float s[] = {0,0};
	float out;
	
	//q is resonance from 0 (low Q), to 1 (high Q)
	float a1 = -2*q*cos(2*3.14159*fc/48000.);
	float a2 = q * q;

	out = in - a1 * s[0] - a2 * s[1];

	

	//y[n-2] = y[n-1]
	//y[n-1] = y[n]
	s[1] = s[0];
	s[0] = out;
	
	return out;

}

float KeyToFreq(uint8_t note){
	if(1==note){return 65.41;} 			//C2
	else if(2==note){return 73.42;}	//D2
	else if(3==note){return 82.41;}	//E2
	else if(4==note){return 98.0;}	//G2
	else if(5==note){return 110.0;}	//A2
	else if(6==note){return 130.81;}	//C3
	else if(7==note){return 146.83;}	//D3
	else if(8==note){return 164.81;}	//E3
	else if(9==note){return 196.00;}	//G3
	else{ return 0;}
	
}

float Distortion (float in){
	
	in = in * in * in;
	if(in > 1){
		in = 1;
	} else if( in < -1){
		in = -1;
	}

return in;
}
