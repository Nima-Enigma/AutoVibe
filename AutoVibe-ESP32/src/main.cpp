#include "credentials.h"
#include "wifi_controller.h"
#include "rpm_controller.h"
#include "settings_handler.h"
#include "audio_controller.h"
#include "ble_controller.h"

static unsigned long lastMsg = 0;
static String rpmCmd = "010C";
static String rpmResp = "410C";

bool busInit() {
    std::vector<JsonObject> initCommands = getInitCommandList();

    for (JsonObject& cmd : initCommands) {
        String cmdText = cmd["cmdtext"] | "";
        int timeWait = cmd["ctimewait"] | 100;
        int trynum = cmd["ctrynum"] | 1;
        String expectedResp = cmd["cresp"] | "";

        if (cmdText.length() == 0) continue;

        if (!cmdText.startsWith("AT") && !activeCarProfile.isCanProtocol() && 
            activeCarProfile.getAddress(cmdText).length() > 0 && activeCarProfile.eid != 1 &&
            activeCarProfile.baudType != 3 && activeCarProfile.lastEcuAddress.length() == 0) {
            activeCarProfile.lastEcuAddress = activeCarProfile.getAddress(cmdText);
            sendOBDCommand("AT SH " + activeCarProfile.lastEcuAddress, 50, 2, "OK");
            sendOBDCommand("AT SP " + (String) activeCarProfile.baudType, 50, 2, "OK");
        }

        if (!sendOBDCommand(cmdText, timeWait, 2, expectedResp) && cmdText.startsWith("AT")) {
            Serial.printf("[INIT] Failed initializing command: '%s'\n", cmdText.c_str());
            activeCarProfile.lastEcuAddress = "";
            return false;
        }
    }

    std::vector<JsonObject> rpmCommands = getRPMParameterObjects();

    for (JsonObject& cmd : rpmCommands) {
        String cmdText = cmd["cmdtext"] | "";
        int timeWait = cmd["ctimewait"] | 100;
        int trynum = cmd["ctrynum"] | 1;
        String expectedResp = cmd["cresp"] | "";

        if (cmdText.length() == 0) continue;

        if (sendOBDCommand(cmdText, timeWait, trynum, expectedResp, true)) {
            rpmCmd = cmdText;
            rpmResp = expectedResp;
            break;
        }
    }

    return true;
}

void setup() {
    Serial.begin(115200);

    setupWiFi(ssid, password, mqttServer, mqttPort);
    setupBLE(macAddress);
    setupRPM();
    setupAudio();
    applyCarProfile("Ford Mustang 69'");

    loadCarJson(ecuConfigPath);
    while(!busInit()) delay(500);
}

void loop() {
    checkOBDConnection();
    checkMQTTConnection(mqttClientID, mqttUser, mqttPass, controlTopic);
    
    int rpm = getRPM();
    Serial.println("Current RPM: " + String(rpm));
    
    if (millis() - lastMsg > 500) {
        lastMsg = millis();
        sendOBDCommand(rpmCmd, 50, 2, rpmResp, true);
        publishRPM(rpm, rpmTopic);
    }
    
    updateAudio(rpm);
}
