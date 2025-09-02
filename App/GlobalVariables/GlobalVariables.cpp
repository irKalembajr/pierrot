// GlobalVariables.cpp (à créer pour éviter multiples définitions)
#include "GlobalVariables.h"

bool sensor1Triggered = false;
bool sensor2Triggered = false;
bool sensor3Triggered = false;

bool loraAlertReceived = false;
unsigned long lastSensor1Time = 0;
unsigned long lastSensor2Time = 0;
unsigned long lastSensor3Time = 0;

unsigned long lastLoRaMessageTime = 0;
String lastLoRaMessage = "";
