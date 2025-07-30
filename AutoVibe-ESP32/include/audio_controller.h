#pragma once
#include <ArduinoJson.h>
#include <driver/i2s.h>
#include <SPIFFS.h>

#define I2S_NUM         I2S_NUM_0
#define BASE_SAMPLE_RATE 22050
extern float BASE_RPM;

#define CROSSFADE_START 3000.0f
#define CROSSFADE_END   6500.0f
#define WAV_HEADER_SIZE 44

#define I2S_DIN  27
#define I2S_BCLK  26
#define I2S_LRC   25

extern File fileLow, fileHigh;
extern float baseVolume;
extern float volumeScale;

void setupAudio();
void updateAudio(int rpm);
void setVolumeLevel(int level);
void applyCarProfile(String car);