
#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>

// Variables globales accessibles partout
extern String wifiSSID;
extern String wifiPASS;
extern String siteName;

// Fonctions à utiliser dans ton main.ino
void setupWiFiManager();
void wifiManagerLoop();

#endif
