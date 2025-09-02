#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <Arduino.h>

// Broches des relais et interrupteur (adapte si besoin)
#define RELAY1   34
#define RELAY2   35
#define SWITCH1  32   // entrée bouton/interrupteur (active LOW si cablé vers GND)

extern bool relay1State;
extern bool relay2State;

#endif
