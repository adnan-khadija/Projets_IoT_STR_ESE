#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>

// Définition des paramètres Wi-Fi
const char* ssid = "has";           // Nom du réseau Wi-Fi
const char* password = "12341234@"; // Mot de passe du réseau Wi-Fi

// Fonction pour se connecter au Wi-Fi
void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Déjà connecté au Wi-Fi !");
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.localIP());
    return;
  }

  Serial.println("Connexion au Wi-Fi...");
  WiFi.begin(ssid, password);

  unsigned long startTime = millis(); // Enregistrer le temps de début de connexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);  // Attendre 500ms avant de vérifier à nouveau
    Serial.print("...");

    // Timeout après 10 secondes
    if (millis() - startTime > 10000) {
      Serial.println("\nErreur : Impossible de se connecter au Wi-Fi !");
      return;
    }
  }

  Serial.println("\nConnecté au Wi-Fi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

#endif
