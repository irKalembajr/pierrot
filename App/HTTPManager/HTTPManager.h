#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <Arduino.h>

HTTPClient http;
http.begin("https://YOUR_DOMAIN/api/state");
http.addHeader("Content-Type", "application/json");
http.addHeader("Authorization", "Bearer mon_token_ultra_secret"); // optionnel
String body = String("{"relay1":") + (relay1State ? "true" : "false") +
              ","relay2":" + (relay2State ? "true" : "false") +
              ","switch":" + ((digitalRead(SWITCH1)==LOW) ? "true":"false") +
              ","source":"esp32"}";
http.POST(body);
http.end();

// URL = https://<your-vercel-domain>/api/command?mode=pop
HTTPClient http;
http.begin("https://pierrot-monitoring.vercel.app/api/command?mode=pop");
http.addHeader("Authorization", "Bearer mon_token_ultra_secret"); // optionnel
int code = http.GET();
if (code == 200) {
  String payload = http.getString(); // JSON: { "command": { "relay1": true/false, "relay2": true/false, "at": "..." } }
  // Parser et appliquer si non nul
}
http.end();

// Envoie l'état au backend (appel immédiat)
void sendRelayState(bool relay1, bool relay2, bool localSwitch, const String& reason);

// À appeler régulièrement dans loop() (gère heartbeat périodique)
void httpManagerLoop(bool relay1, bool relay2, bool localSwitch);

#endif
