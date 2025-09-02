#include "HTTPManager.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

static unsigned long lastHeartbeat = 0;

void sendRelayState(bool relay1, bool relay2, bool localSwitch, const String& reason) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi non connecté, impossible d'envoyer l'état");
        return;
    }

    WiFiClientSecure client;
    client.setInsecure();  // ⚠️ désactive la vérification SSL (OK pour dev, pas en prod)

    HTTPClient http;
    http.begin(client, "https://ton-dashboard.vercel.app/api/state"); // <-- mets ton URL ici
    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    payload += "\"relay1\":" + String(relay1 ? "true" : "false") + ",";
    payload += "\"relay2\":" + String(relay2 ? "true" : "false") + ",";
    payload += "\"localSwitch\":" + String(localSwitch ? "true" : "false") + ",";
    payload += "\"reason\":\"" + reason + "\"";
    payload += "}";

    int code = http.POST(payload);
    Serial.print("[HTTP] POST code: ");
    Serial.println(code);

    if (code > 0) {
        String response = http.getString();
        Serial.println("Réponse serveur: " + response);
    }

    http.end();
}

void httpManagerLoop(bool relay1, bool relay2, bool localSwitch) {
    unsigned long now = millis();
    if (now - lastHeartbeat >= 10000) { // toutes les 10 secondes
        lastHeartbeat = now;
        sendRelayState(relay1, relay2, localSwitch, "heartbeat");
    }
}
