#pragma once

// WiFi credentials
const char* ssid = "...";
const char* password = "...";

// MQTT broker
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPass = "";
const char* mqttClientID = "ESP32{cc727180-d9f1-4df7-893e-0dfb1b96ae7e}";

// Topics
const char* rpmTopic = "car{cc727180-d9f1-4df7-893e-0dfb1b96ae7e}/rpm";
const char* controlTopic = "car{cc727180-d9f1-4df7-893e-0dfb1b96ae7e}/control";

// OBD2
const char* obd2UUID = "...";
const char* macAddress = "...";

// ECU
const char* ecuConfigPath = "/ecu_config.json";