#include "wifi_controller.h"
#include "settings_handler.h"

WiFiClient espClient;
PubSubClient client(espClient);

void reconnectMQTT(const char* mqttClientID, const char* mqttUser, const char* mqttPass, const char* controlTopic) {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT... ");
        if (client.connect(mqttClientID, mqttUser, mqttPass)) {
            Serial.println("connected!");
            client.subscribe(controlTopic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 3 seconds...");
            delay(3000);
        }
    }
}

void mqttCallback(char* topic, byte* message, unsigned int length) {
    String payload;
    for (unsigned int i = 0; i < length; i++) {
        payload += (char)message[i];
    }
    Serial.printf("Message [%s]: %s\n", topic, payload.c_str());

    if (((std::string) topic).find("control") != std::string::npos) {
        handleSettingsMessage(payload.c_str());
    }
}

void setupWiFi(const char* ssid, const char* password, const char* mqttServer, int mqttPort) {
    WiFi.begin(ssid, password);
    Serial.printf("Connecting to %s ", ssid);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    client.setServer(mqttServer, mqttPort);
    client.setCallback(mqttCallback);
}

void checkMQTTConnection(const char* mqttClientID, const char* mqttUser, const char* mqttPass, const char* controlTopic) {
    if (!client.connected()) {
        reconnectMQTT(mqttClientID, mqttUser, mqttPass, controlTopic);
    }
    client.loop();
}