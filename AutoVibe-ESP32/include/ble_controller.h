#pragma once
#include <Arduino.h>
#include <BLEDevice.h>

extern BLEClient* bleClient;
extern BLERemoteCharacteristic* bleTxCharacteristic;
extern BLERemoteCharacteristic* bleRxCharacteristic;

extern BLEAddress obdAddress;
extern bool obdConnected;

bool connectToOBD();
bool findAndConnectOBD(const char* deviceAddress);
void setupBLE(const char* deviceAddress);
void checkOBDConnection();
bool sendOBDCommand(const String& cmd, int timeoutMs, int numRetries, const String& succResp, bool isRPM=false);