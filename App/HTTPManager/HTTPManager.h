#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H

#include <Arduino.h>

// Envoi immédiat de l’état au dashboard
void sendRelayState(bool relay1, bool relay2, bool localSwitch, const String& reason);

// Boucle périodique (heartbeat)
void httpManagerLoop(bool relay1, bool relay2, bool localSwitch);

#endif
