#ifndef FIREBASE_H
#define FIREBASE_H

#include <FirebaseESP8266.h>


#define FIREBASE_HOST "https://feux-rouge-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyDvbT6VdbciqK6BKJ8hk4uAtUmtMYLokFM"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

void initFirebase() {
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);

  // Tentative de connexion WiFi à Firebase
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connexion Wi-Fi réussie !");
    // Firebase is initialized, but make sure WiFi is connected first
    Serial.println("Connexion à Firebase réussie !");
  } else {
    Serial.println("Erreur de connexion Wi-Fi. Impossible de se connecter à Firebase.");
  }
}

#endif
