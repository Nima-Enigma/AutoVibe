#include "settings_handler.h"
#include "audio_controller.h"

DynamicJsonDocument carConfig(40000);
CarProfile activeCarProfile;

void handleSettingsMessage(const char* payload) {
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        Serial.print("[Settings] Failed to parse JSON: ");
        Serial.println(error.c_str());
        return;
    }

    if (doc.containsKey("volume_level")) {
        int volume = doc["volume_level"];
        Serial.printf("[Settings] Set volume level to: %d\n", volume);
        setVolumeLevel(volume);
    }

    if (doc.containsKey("chosen_car")) {
        String car = doc["chosen_car"].as<String>();
        Serial.printf("[Settings] Selected car profile: %s\n", car.c_str());
        applyCarProfile(car);
    }
}

bool loadCarJson(const char* path) {
    if (!SPIFFS.begin(true)) {
        Serial.println("[Settings]/[ECU] Failed to mount SPIFFS");
        return false;
    }

    File file = SPIFFS.open(path, "r");
    if (!file) {
        Serial.printf("[Settings]/[ECU] Failed to open JSON file: %s\n", path);
        return false;
    }

    DeserializationError error = deserializeJson(carConfig, file);
    file.close();

    if (error) {
        Serial.print("[Settings]/[ECU] JSON parsing failed: ");
        Serial.println(error.c_str());
        return false;
    }

    Serial.println("[Settings]/[ECU] JSON loaded and parsed.");

    JsonObject specs = carConfig["CONNECTION"]["SPECS"].as<JsonObject>();
    activeCarProfile.fromJson(specs);
    return true;
}

std::vector<JsonObject> getInitCommandList() {
    std::vector<JsonObject> commandObjects;

    JsonArray cmdArray = carConfig["CONNECTION"]["INITCMDLIST"].as<JsonArray>();
    for (JsonObject cmd : cmdArray) {
        commandObjects.push_back(cmd);
    }

    return commandObjects;
}

std::vector<JsonObject> getRPMParameterObjects() {
    std::vector<JsonObject> rpmParams;

    JsonObject parameterMap = carConfig["PARAMETER"].as<JsonObject>();
    for (JsonPair keyValue : parameterMap) {
        JsonArray paramArray = keyValue.value().as<JsonArray>();
        for (JsonObject paramObj : paramArray) {
            String unitSim = paramObj["unit_sim"] | "";
            if (unitSim.equalsIgnoreCase("rpm")) {
                rpmParams.push_back(paramObj);
            }
        }
    }

    return rpmParams;
}