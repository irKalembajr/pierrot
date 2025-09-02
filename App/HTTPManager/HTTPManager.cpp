#include "HTTPManager.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "WiFiManager.h"      // pour extern String siteName
#include "OLEDManager.h"      // pour displayMessage()

// === CONFIG CIBLE VERCEL ===
// Mets ici ton endpoint Vercel (HTTPS)
static const char* ENDPOINT_URL = "https://pierrot-monitoring.vercel.app/api/data";

// Heartbeat (en ms) : envoie l’état même si rien n’a changé
static const unsigned long HEARTBEAT_MS = 60000UL;

static unsigned long lastHeartbeat = 0;

// --- Envoi HTTP brut ---
static bool postJSON(const String& url, const String& json, int& httpCode, String& body) {
  if (WiFi.status() != WL_CONNECTED) {
    httpCode = -1;
    body = "WiFi not connected";
    return false;
  }

  WiFiClientSecure client;
  client.setTimeout(15000);
  // Rapide à tester : pas de vérification de certificat
  // (pour production, remplace par client.setCACert(...))
  client.setInsecure();

  HTTPClient http;
  if (!http.begin(client, url)) {
    httpCode = -2;
    body = "begin() failed";
    return false;
  }

  http.addHeader("Content-Type", "application/json");

  httpCode = http.POST(json);
  body = http.getString();
  http.end();

  return (httpCode > 0 && httpCode < 400);
}

// --- Construit et envoie le JSON d’état ---
void sendRelayState(bool relay1, bool relay2, bool localSwitch, const String& reason) {
  if (WiFi.status() != WL_CONNECTED) {
    displayMessage("HTTP", "WiFi OFF");
    Serial.println("[HTTP] Annulé: WiFi non connecté");
    return;
  }

  // JSON minimal et cohérent avec le backend
  // Ajoute ce dont tu as besoin côté Vercel.
  String json = "{";
  json += "\"site\":\"" + siteName + "\",";
  json += "\"type\":\"etat_relais\",";
  json += "\"relay1\":" + String(relay1 ? "true" : "false") + ",";
  json += "\"relay2\":" + String(relay2 ? "true" : "false") + ",";
  json += "\"switch\":" + String(localSwitch ? "true" : "false") + ",";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"reason\":\"" + reason + "\"";
  json += "}";

  int code = 0;
  String payload;
  bool ok = postJSON(ENDPOINT_URL, json, code, payload);

  Serial.println("[HTTP] POST " + String(ENDPOINT_URL));
  Serial.println("[HTTP] JSON: " + json);
  Serial.println("[HTTP] Code: " + String(code));
  Serial.println("[HTTP] Resp: " + payload);

  displayMessage(ok ? "Etat envoyé" : "Erreur HTTP", "code: " + String(code));
  lastHeartbeat = millis(); // reset le timer de heartbeat après un envoi
}

// --- Heartbeat périodique ---
void httpManagerLoop(bool relay1, bool relay2, bool localSwitch) {
  unsigned long now = millis();
  if (now - lastHeartbeat >= HEARTBEAT_MS) {
    sendRelayState(relay1, relay2, localSwitch, "heartbeat");
    // lastHeartbeat est remis à jour dans sendRelayState()
  }
}
