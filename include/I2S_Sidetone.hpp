#pragma once
#include "AudioTools.h"

class I2S_Sidetone {
	public:
		I2S_Sidetone();
		void begin();
		void setFrequency(float f);
		float getFrequency();
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
		StreamCopy *copier;
		ADSRGain *adsr;
		float frequency;
};
