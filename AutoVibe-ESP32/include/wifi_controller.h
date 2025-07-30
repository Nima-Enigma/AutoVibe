#pragma once
#include <WiFi.h>
#include <PubSubClient.h>

extern WiFiClient espClient;
extern PubSubClient client;

void reconnectMQTT(const char* mqttClientID, const char* mqttUser, const char* mqttPass, const char* controlTopic);
void mqttCallback(char* topic, byte* message, unsigned int length);
void setupWiFi(const char* ssid, const char* password, const char* mqttServer, int mqttPort);
void checkMQTTConnection(const char* mqttClientID, const char* mqttUser, const char* mqttPass, const char* controlTopic);