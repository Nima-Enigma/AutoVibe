#pragma once
#include <ArduinoJson.h>
#include <vector>

extern DynamicJsonDocument carConfig;

struct CarProfile {
    String lastEcuAddress = "";

    int Sou = 0;
    int baudRate = 0;
    int baudType = 0;
    bool changeBaud = false;
    String deviceVersion = "";
    String fileName = "";
    int historyId = 0;
    int initTry = 0;
    int init_group_id = 0;
    int menu_level = 0;

    int f162p = 0;
    int reInitTiming = 0;
    int t65 = 0;
    int t74 = 0;
    int wakeupTiming = 0;

    String f163wc = "";  
    int f164wt = 0;
    String attribute = "";
    String STS = "";

    String f157K1 = "";
    String lastTarget = "";

    String f158K2 = "";
    bool isUpASA3Sharp = false;
    int countNoRespDevice[3] = {0, 0, 0};
    String bufferSession = "";
    int ver_Db = 1;
    String AWF_MultiLine = "AT 87 00 01 ";

    String f160d = "";

    int f159c = 0;
    int eid = 0;

    String f161dw = "";
    int type = 1;
    String typeW = "1";

    void fromJson(JsonObject specs) {
        wakeupTiming = specs["wakeupTiming"] | 0;
        reInitTiming = specs["reInitTiming"] | 0;

        f160d = specs["d"] | "";
        type = specs["type"] | 1;
        typeW = specs["typew"] | "";
        f161dw = specs["dw"] | "";
        f163wc = specs["wc"] | "";
        baudType = specs["baudtype"] | 0;

        String attr = specs["attribute"] | "";
    }

    bool isCanProtocol() {
        return (type == 4 || type == 5);
    }

    String getAddress(const String& cmd) {
        if (f161dw != "" && !cmd.startsWith("81")) {
            return f161dw;
        }
        return f160d;
    }
};

extern CarProfile activeCarProfile;

void handleSettingsMessage(const char* payload);
bool loadCarJson(const char* path="/ecu_config.json");
std::vector<JsonObject> getInitCommandList();
std::vector<JsonObject> getRPMParameterObjects();