#include "rpm_controller.h"
#include "wifi_controller.h"

int currRPM = 0;

void setupRPM() {
    // In case you wanted to use a Potentiometer instead of car's ECU
    // pinMode(RPM_INPUT_PIN, INPUT);
}

void updateRPM(const String& response) {
    String clean = response;
    clean.replace(" ", "");
    clean.toUpperCase();

    int idx = clean.indexOf("410C");
    if (idx == -1 || clean.length() < idx + 8) {
        return;
    }

    String A_str = clean.substring(idx + 4, idx + 6);
    String B_str = clean.substring(idx + 6, idx + 8);

    int A = strtol(A_str.c_str(), nullptr, 16);
    int B = strtol(B_str.c_str(), nullptr, 16);

    currRPM = ((A * 256) + B) / 4;
}

int getRPM() {
    static int smoothedRPM = 0;
    
    // In case you wanted to use a Potentiometer instead of car's ECU
    // int rawValue = analogRead(RPM_INPUT_PIN);
    // int currentRPM = map(rawValue, 0, 4095, 0, MAX_RPM);

    smoothedRPM = (smoothedRPM * 9 + currRPM) / 10;

    return smoothedRPM;
}

void publishRPM(int rpm, const char* rpmTopic) {
    StaticJsonDocument<64> doc;
    doc["rpm"] = rpm;
    String rpmJson;
    serializeJson(doc, rpmJson);

    client.publish(rpmTopic, rpmJson.c_str());
    Serial.println("[RPM] Published RPM: " + rpmJson);
}