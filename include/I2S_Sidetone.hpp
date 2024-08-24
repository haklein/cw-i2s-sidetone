#pragma once
#include "AudioTools.h"

class I2S_Sidetone {
	public:
		I2S_Sidetone();
		void begin(int buffer_size=32);
		void setFrequency(float f);
		void setVolume(float v);
		float getFrequency();
		void setADSR(float attack, float decay, float sustainLevel, float release);
		void on();
		void off();
		bool isOn();
		void tick();
	private:
		I2SStream *i2s;
		SineFromTable<int16_t> *sine;
		// SineWaveGenerator<int16_t> *sine;
		GeneratedSoundStream<int16_t> *in; 
		AudioEffectStream *effects;
		VolumeStream *volume;
		StreamCopy *copier;
		ADSRGain *adsr;
		float frequency;
};
