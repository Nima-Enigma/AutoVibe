#include "audio_controller.h"

float BASE_RPM = 1000.0f;

File fileLow, fileHigh;
float baseVolume = 0.5f;
float volumeScale = 1.0f;

void setupAudio() {
    if (!SPIFFS.begin(true)) {
        Serial.println("[AUDIO] SPIFFS mount failed!");
        return;
    }

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = BASE_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_out_num = I2S_DIN,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM, &pin_config);
    i2s_zero_dma_buffer(I2S_NUM);

    Serial.println("[AUDIO] I2S initialized");
}

void updateAudio(int rpm) {
    float pitchRatio = (float) 1.0f + (rpm / BASE_RPM) * 0.4f;
    int newSampleRate = (int)(BASE_SAMPLE_RATE * pitchRatio);
    i2s_set_sample_rates(I2S_NUM, newSampleRate);

    float t = constrain((float)(rpm - CROSSFADE_START) / (CROSSFADE_END - CROSSFADE_START), 0.0f, 1.0f);
    float gainLow = cosf((1.0f - t) * 0.5f * PI);
    float gainHigh = cosf(t * 0.5f * PI);

    const int bufferSize = 512;
    uint8_t bufferLow[bufferSize] = {0};
    uint8_t bufferHigh[bufferSize] = {0};

    int bytesReadLow = fileLow.read(bufferLow, bufferSize);
    int bytesReadHigh = fileHigh.read(bufferHigh, bufferSize);

    if (bytesReadLow <= 0) {
        fileLow.seek(WAV_HEADER_SIZE);
        bytesReadLow = fileLow.read(bufferLow, bufferSize);
    }
    if (bytesReadHigh <= 0) {
        fileHigh.seek(WAV_HEADER_SIZE);
        bytesReadHigh = fileHigh.read(bufferHigh, bufferSize);
    }

    int bytesRead = min(bytesReadLow, bytesReadHigh);
    int16_t mixedBuffer[bufferSize];

    for (int i = 0; i < bytesRead; i++) {
        int16_t sampleLow = ((int16_t)bufferLow[i] - 128) << 8;
        int16_t sampleHigh = ((int16_t)bufferHigh[i] - 128) << 8;
        int16_t mixed = (int16_t)((sampleLow * gainLow + sampleHigh * gainHigh) * baseVolume * volumeScale);
        mixedBuffer[i] = mixed;
    }

    size_t bytesWritten;
    i2s_write(I2S_NUM, mixedBuffer, bytesRead * sizeof(int16_t), &bytesWritten, portMAX_DELAY);
}

void setVolumeLevel(int level) {
    volumeScale = constrain(level, 0, 100) / 100.0f;
    Serial.printf("[AUDIO] Volume level set to %d%%\n", level);
}

void applyCarProfile(String car) {
    File configFile = SPIFFS.open("/car_profiles.json", "r");
    if (!configFile) {
        Serial.println("[AUDIO]/[PROFILE] Failed to open car_profiles.json");
        return;
    }

    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size + 1]);
    configFile.readBytes(buf.get(), size);
    buf[size] = '\0';

    DynamicJsonDocument doc(1024);
    DeserializationError err = deserializeJson(doc, buf.get());
    if (err) {
        Serial.println("[AUDIO]/[PROFILE] JSON parse failed");
        return;
    }

    JsonObject carData = doc[car];
    if (carData.isNull()) {
        Serial.println("[AUDIO]/[PROFILE] Car not found in config");
        return;
    }

    String lowFile = carData["low"];
    String highFile = carData["high"];
    BASE_RPM = (float) carData["base_rpm"];
    baseVolume = (float) carData["volume"];

    if (fileLow) fileLow.close();
    if (fileHigh) fileHigh.close();

    fileLow = SPIFFS.open(lowFile, "r");
    fileHigh = SPIFFS.open(highFile, "r");

    if (fileLow) fileLow.seek(WAV_HEADER_SIZE);
    else Serial.println("[AUDIO]/[PROFILE] Failed to open low file");

    if (fileHigh) fileHigh.seek(WAV_HEADER_SIZE);
    else Serial.println("[AUDIO]/[PROFILE] Failed to open high file");

    Serial.printf("[AUDIO]/[PROFILE] Loaded car '%s'\n", car.c_str());
}