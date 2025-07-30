# AutoVibe 🚗🔊

A Real-Time Engine Sound Simulator for Electric Vehicles, powered by ESP32, BLE (ELM327), and MQTT.

## 🌟 Overview

Electric vehicles lack the familiar engine sound, which can impact driving experience and pedestrian safety. AutoVibe simulates engine sounds based on real-time RPM (Revolutions Per Minute) data, creating a more immersive and safer driving environment.

This system:
- Reads the RPM from a car's OBD2 port via an ELM327 BLE adapter.
- Synthesizes and plays appropriate engine sound based on the RPM.
- Allows configuration (e.g., car type, volume) from a custom Android app via MQTT.

---

## 🧩 Components

### Hardware
- **ESP32 WROOM32D**
- **MAX98357 I2S Audio Amplifier Module**
- **8Ω 1W Speaker**
- **ELM327 BLE Adapter**
- **Jumper wires**

### Software
- ESP32 firmware written in **C++**
- Android app written in **Java** using **MQTT**
- Uses [Sisoog's SICAR-Diag](https://github.com/Sisoog/sicar-diag) JSON protocol for ECU interaction

---

## 🛠️ Setup Guide

### 1. Flash ESP32
- Open the project in **PlatformIO** or **Arduino IDE**
- Adjust WiFi and MQTT configurations inside the code:

  ```cpp
  const char* ssid = "YOUR_WIFI_SSID";
  const char* password = "YOUR_WIFI_PASSWORD";
  const char* mqtt_server = "test.mosquitto.org";
  const char* obd2UUID = "YOUR_OBD2_DEVICE_UUID";
  const char* macAddress = "YOUR_ELM327_MAC_ADDRESS";
- Upload the code to your ESP32

### 2. Connect Hardware
Connect ESP32 to the MAX98357 as follows:

- LRC → IO25
- BCLK → IO26
- DIN → IO27
- VDD → 5V
- GND → GND

Connect speaker to the output pins of MAX98357

Pair ELM327 BLE with car's OBD2 port

### 3. Use Android App
Clone or download the Android project (available in the repo)

Open in Android Studio

Set the same MQTT server as in the ESP32 code

Build and install the app

Use the app to:

- View live RPM
- Select vehicle profile
- Adjust volume

## 📦 File Structure
  ```bash
  AutoVibe/
  │
  ├── AutoVibe-ESP32/                   # Firmware for ESP32 board
  │   ├── src/
  │   └── lib/
  │
  ├── AutoVibe-Android/              # Android application source code
  │
  └── README.md
  ```

## 📌 Notes for Users

- The project uses BLE connection to read RPM. Connection may be unstable with some cheap ELM327 adapters.
- If your car’s ECU is incompatible with SICAR's JSON files, the system will fall back to the standard OBD2 command `010C`.
- Sound processing uses Crossfade and PitchShift, which may introduce minor latency.
- _**MQTT messages are sent to public brokers. If security is a concern, consider deploying a private broker.**_

## ❗ Limitations

- BLE connection stability may vary with ELM327 adapters.
- Real-time performance is limited due to ESP32 and BLE communication bottlenecks.
- ECU command compatibility relies on external JSON profiles.
- Sound quality can be affected by hardware and processing delays.
- MQTT security and latency depend on the broker you choose (default is a public one).

## ✅ Future Improvements

- Integrate secure MQTT broker with authentication.
- Add noise cancellation and DSP enhancements for better sound clarity.
- Include dynamic vehicle profiles and more realistic sound morphing.
- Improve BLE handling and ECU compatibility.

## 👥 Contributors
[**Nikan Vasei**](https://github.com/NikanV/) - [**Nima Miri**](https://github.com/Nima-Enigma/)

## 📄 License
This project is licensed under the MIT [License](./LICENSE).
