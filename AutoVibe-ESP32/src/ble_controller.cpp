#include "ble_controller.h"
#include "rpm_controller.h"

BLEClient* bleClient = nullptr;
BLERemoteCharacteristic* bleTxCharacteristic = nullptr;
BLERemoteCharacteristic* bleRxCharacteristic = nullptr;

BLEAddress obdAddress("");
bool obdConnected = false;

String lastOBDResponse = "";
volatile bool responseReceived = false;
volatile bool waitResp = true;

bool connectToOBD() {
    if (!bleClient)
        bleClient = BLEDevice::createClient();

    Serial.println("[BLE] Connecting to OBD device at: " + (String) obdAddress.toString().c_str());
    if (!bleClient->connect(obdAddress)) {
        Serial.println("[BLE] Failed to connect");
        return false;
    }

    Serial.println("[BLE] Connected! Discovering services...");

    BLERemoteService* bleService = bleClient->getService(BLEUUID("FFF0"));
    if (bleService == nullptr) {
        Serial.println("[BLE] Failed to find OBD service.");
        return false;
    }

    std::map<std::string, BLERemoteCharacteristic*>* charMap = bleService->getCharacteristics();
    for (auto it = charMap->begin(); it != charMap->end(); ++it) {
        BLERemoteCharacteristic* c = it->second;

        if (c->canNotify()) bleTxCharacteristic = c;
        if (c->canWrite() || c->canWriteNoResponse()) bleRxCharacteristic = c;
    }

    if (bleTxCharacteristic->canNotify()) {
        bleTxCharacteristic->registerForNotify([](BLERemoteCharacteristic* pChar, uint8_t* data, size_t len, bool isNotify) {
            lastOBDResponse = "";
            for (size_t i = 0; i < len; i++) {
                lastOBDResponse += (char)data[i];
            }
            if (!waitResp) updateRPM(lastOBDResponse);
            else responseReceived = true;
        });
    }

    if (bleTxCharacteristic && bleRxCharacteristic) {
        obdConnected = true;
        Serial.println("[BLE] OBD TX and RX characteristics set.");
        return true;
    }

    Serial.println("[BLE] Could not find TX/RX characteristics");
    return false;
}

bool findAndConnectOBD(const char* deviceAddress) {
    if (deviceAddress != "") {
        obdAddress = BLEAddress(deviceAddress);
        Serial.println("[BLE] Using provided OBD address");
        return connectToOBD();
    }

    Serial.println("[BLE] Starting BLE scan...");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);
    BLEScanResults results = pBLEScan->start(5);

    for (int i = 0; i < results.getCount(); ++i) {
        BLEAdvertisedDevice d = results.getDevice(i);
        String name = d.getName().c_str();
        if (name.indexOf("OBD") != -1) {
            Serial.println("[BLE] Found OBD device: " + name);
            obdAddress = d.getAddress();
            return connectToOBD();
        }
    }

    Serial.println("[BLE] No OBD device found.");
    return false;
}

void setupBLE(const char* deviceAddress) {
    BLEDevice::init("AutoVibe BLE Client");
    while (findAndConnectOBD(deviceAddress) == false) {
        Serial.println("[BLE] OBD connection failed, retrying in 3 seconds...");
        delay(3000);
    }
}

void checkOBDConnection() {
    if (!obdConnected || (bleClient && !bleClient->isConnected())) {
        Serial.println("[BLE] OBD disconnected, retrying...");
        obdConnected = false;
        findAndConnectOBD(obdAddress.toString().c_str());
    }
}

bool sendOBDCommand(const String& cmd, int timeoutMs, int numRetries, const String& succResp, bool isRPM) {
    if (!obdConnected || bleRxCharacteristic == nullptr) {
        Serial.println("[BLE] OBD disconnected or write characteristic missing.");
        return false;
    }
    
    timeoutMs += 2000;
    lastOBDResponse = "";
    responseReceived = false;

    String fullCmd = cmd + "\r";
    bleRxCharacteristic->writeValue(fullCmd.c_str(), false);
    Serial.print("[BLE] Sent to ELM327: ");
    Serial.println(fullCmd);
    
    u_long startTime = millis();
    while (waitResp && (millis() - startTime < timeoutMs)) {
        if (responseReceived) {
            if (lastOBDResponse.indexOf("UNABLE") != -1 ||
                lastOBDResponse.indexOf("NO DATA") != -1) {
                return false;
            }

            if (isRPM) {
                lastOBDResponse.replace(" ", "");
                if (!lastOBDResponse.startsWith(succResp)) return false;
                else {
                    waitResp = false;
                    updateRPM(lastOBDResponse);
                }
            }

            return true;
        }
        delay(1);
    }
    if (numRetries > 0) {
        Serial.println("[BLE] Timed out, Retrying...");
        return sendOBDCommand(cmd, timeoutMs, numRetries - 1, succResp, isRPM);
    }

    return false;
}