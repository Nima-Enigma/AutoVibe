#pragma once
#include <ArduinoJson.h>

#define RPM_INPUT_PIN 34      
#define MAX_RPM 8000

extern int currRPM;

void updateRPM(const String& response);
void setupRPM();
int getRPM();
void publishRPM(int rpm, const char* rpmTopic);