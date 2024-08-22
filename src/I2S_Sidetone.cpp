#include "I2S_Sidetone.hpp"

static void audio_task(void *userData)
{
  StreamCopy *copier = (StreamCopy*)userData;
  // Serial.println("copier task running");
  // Serial.print("Stream Copier status:");
  // Serial.println(copier->isActive());
  while (1) {
    copier->copy();
    // taskYIELD();
  }
}

I2S_Sidetone::I2S_Sidetone() {
    frequency=0.0;
}
void I2S_Sidetone::begin() {  
    AudioLogger::instance().begin(Serial,AudioLogger::Info);
    i2s = new I2SStream;
    I2SConfig config = i2s->defaultConfig(TX_MODE);
    config.sample_rate = 44100; 
    config.channels = 2;
    config.bits_per_sample = 16;
    config.pin_bck = CONFIG_I2S_BCK_PIN; // define your i2s pins
    config.pin_ws = CONFIG_I2S_LRCK_PIN;
    config.pin_data = CONFIG_I2S_DATA_PIN;
    config.buffer_size=1024;

    // Serial.println("starting I2S...");
    i2s->begin(config);

        
    sine = new SineFromTable<int16_t>();
    // sine = new SineWaveGenerator<int16_t>();
	in = new GeneratedSoundStream<int16_t>(*sine); 

    volume = new VolumeStream(*i2s);
    effects = new AudioEffectStream(*in);
    copier = new StreamCopy(*volume, *effects);
    adsr = new ADSRGain(0.0100,20.0, 1.0 , 0.008);

    float freq = 600.0;
    sine->begin(config, 0);
    sine->setFrequency(freq);
    in->begin(config);

    effects->addEffect(*adsr);
    effects->begin(config);

    volume->begin(config);
    volume->setVolume(0.3);

    AudioLogger::instance().begin(Serial,AudioLogger::Error);
    xTaskCreatePinnedToCore(audio_task, "audio", 4096, (void*)copier, configMAX_PRIORITIES - 1, nullptr, 1);
}
void I2S_Sidetone::setADSR(float attack=0.001, float decay=0.001, float sustainLevel=0.5, float release=0.005) {
    adsr->setAttackRate(attack);
    adsr->setDecayRate(decay);
    adsr->setReleaseRate(release);
    adsr->setSustainLevel(sustainLevel);
}

void I2S_Sidetone::tick() {
    copier->copy();
}
void I2S_Sidetone::setFrequency(float f) {
    sine->setFrequency(f);
    frequency = f;
}
void I2S_Sidetone::setVolume(float v) {
    volume->setVolume(v);
}
float I2S_Sidetone::getFrequency() {
    return frequency;
}
void I2S_Sidetone::on() {
    adsr->keyOn();
}
void I2S_Sidetone::off() {
    adsr->keyOff();
}
bool I2S_Sidetone::isOn() {
    return false;
}
