#include "WiFiManager.h"

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include <SPIFFS.h>

// Instances globales
WebServer server(80);
DNSServer dnsServer;

// Variables de config
String wifiSSID = "";
String wifiPASS = "";
String siteName = "";

// Prototype de handleSave pour que startAPMode puisse l'utiliser
void handleSave();

// Scan des réseaux WiFi
String scanNetworksHTML() {
  int n = WiFi.scanNetworks();
  String options;
  for (int i = 0; i < n; i++) {
    options += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
  }
  return options;
}

// Page portail captif
void handleRoot() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Configuration Wi-Fi</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color:#009688;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column;
      min-height: 100vh;
      align-items: center;
      justify-content: center;
    }
    .container {
      background: #fff;
      padding: 20px 30px;
      margin-top: 20px;
      border-radius: 10px;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
      max-width: 400px;
      width: 90%;
    }
    h1 {
      text-align: center;
      color: #333;
    }
    label {
      font-weight: bold;
      display: block;
      margin-top: 15px;
    }
    input, select {
      width: 100%;
      padding: 8px;
      margin-top: 5px;
      border-radius: 5px;
      border: 1px solid #ccc;
    }
    input[type="submit"] {
      margin-top: 20px;
      background-color: #007bff;
      color: white;
      border: none;
      cursor: pointer;
    }
    input[type="submit"]:hover {
      background-color: #0056b3;
    }
    .footer {
      margin-top: auto;
      padding: 15px;
      font-size: 0.9em;
      text-align: center;
      color: #666;
    }
    .logo {
      text-align: center;
      margin-bottom: 10px;
    }
    .logo img {
      max-width: 200px;
    }
    .desc {
      text-align: center;
      font-size: 0.9em;
      color: #555;
      margin-bottom: 15px;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="logo">
      <img src="/logo.png" alt="Logo Beni Futur">
    </div>
    <h1>Configuration du systeme</h1>
    <div class="desc">
      Ce système permet la connexion automatique à un réseau Wi-Fi local et l'envoi de données d'alerte via Internet.
    </div>
    <form method="POST" action="/save">
      <label for="ssid">Réseau Wi-Fi (SSID)</label>
      <select name="ssid">
)rawliteral";

  page += scanNetworksHTML();

  page += R"rawliteral(
      </select>
      <label for="password">Mot de passe</label>
      <input type="password" name="password" required>

      <label for="site">Nom du site</label>
      <input type="text" name="site" required>

      <input type="submit" value="Se connecter">
    </form>
  </div>
  <div class="footer">
    © Tous droits réservés - Beni Futur. Conçu par Pierrot BEKWA
  </div>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}

// Sauvegarde de la config dans SPIFFS
void handleSave() {
  if (server.hasArg("ssid") && server.hasArg("password") && server.hasArg("site")) {
    wifiSSID = server.arg("ssid");
    wifiPASS = server.arg("password");
    siteName = server.arg("site");

    File f = SPIFFS.open("/config.txt", "w");
    if (f) {
      f.println(wifiSSID);
      f.println(wifiPASS);
      f.println(siteName);
      f.close();
    }
    server.send(200, "text/html", "<h3>Config saved. Restarting...</h3>");
    delay(1000);
    ESP.restart();
  } else {
    server.send(400, "text/html", "Missing parameters");
  }
}

// Mode point d’accès
void startAPMode() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32_Config", "12345678");
  dnsServer.start(53, "*", WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();

  Serial.println("[WiFiManager] AP mode started. Connect to ESP32_Config");
}

// Connexion au WiFi avec config sauvegardée
bool connectWiFi() {
  if (!SPIFFS.begin(true)) {
    Serial.println("[WiFiManager] SPIFFS init failed");
    return false;
  }

  File f = SPIFFS.open("/config.txt", "r");
  if (!f) return false;

  wifiSSID = f.readStringUntil('\n'); wifiSSID.trim();
  wifiPASS = f.readStringUntil('\n'); wifiPASS.trim();
  siteName = f.readStringUntil('\n'); siteName.trim();
  f.close();

  Serial.println("[WiFiManager] Trying WiFi: " + wifiSSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID.c_str(), wifiPASS.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();

  return (WiFi.status() == WL_CONNECTED);
}

// Initialisation
void setupWiFiManager() {
  if (!connectWiFi()) {
    startAPMode();
  } else {
    Serial.println("[WiFiManager] Connected! IP: " + WiFi.localIP().toString());
  }
}

// Boucle du portail captif
void wifiManagerLoop() {
  if (WiFi.status() != WL_CONNECTED) {
    dnsServer.processNextRequest();
    server.handleClient();
  }
}
