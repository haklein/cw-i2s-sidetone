#pragma once

#include "AudioTools.h"
#include "AudioTools/Disk/AudioSourceSPIFFS.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "BlackmanHarrisEnvelope.hpp"
#include "ComplexRotorSine.hpp"

class I2S_Sidetone {
	public:
    I2S_Sidetone();
    void begin(int samplerate = 44100, int bps=16, int channels = 2, int buffer_size=32);
    void setFrequency(float f);
    void setVolume(float v);
    float getFrequency();
    void setADSR(float attack, float decay, float sustainLevel, float release);
    void on();
    void off();
    bool playSPIFFSFile(const char *filename);
    bool isOn();
    void tick();
    size_t readBytes (uint8_t *data, size_t len);
    int available ();
	private:
    I2SStream *i2s;
    ComplexRotorSine *sine;
    GeneratedSoundStream<int16_t> *in;
    AudioEffectStream *effects;
    VolumeStream *volume;
    LogarithmicVolumeControl *lvc;
    StreamCopy *copier;
    BlackmanHarrisEnvelope *adsr;

    File mp3file;
    EncodedAudioStream *decoder;
    InputMixer<int16_t> *mixer;

    float frequency;
};
