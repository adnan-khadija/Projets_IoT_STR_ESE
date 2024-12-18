#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h> // Inclure si nécessaire pour gérer le WiFi
extern bool inverted;  // Déclaration externe de `inverted`
extern void setLights(); 

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_server = "192.168.26.200"; // Adresse du serveur MQTT
const int mqtt_port = 1883;               // Port MQTT standard
int numVehicles=0;
int numPedestrians=0;
bool ambulanceDetected=false;

// Fonction de callback pour gérer les messages entrants
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message reçu sur le topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
   if (String(topic) == "trafficLight/toggle" && message == "true") {
    inverted = !inverted;
    setLights();  // Appeler la fonction qui gère l'état des feux
  }

  // Traiter les messages selon les topics
  if (String(topic) == "traffic/vehicles") {
    numVehicles = message.toInt();  // Convertir en entier
    Serial.println("Nombre de véhicules mis à jour : " + String(numVehicles));
  } else if (String(topic) == "traffic/pedestrians") {
    numPedestrians = message.toInt();  // Convertir en entier
    Serial.println("Nombre de piétons mis à jour : " + String(numPedestrians));
  } else if (String(topic) == "traffic/ambulances") {
    ambulanceDetected = (message.toInt() > 0);  // Priorité si au moins une ambulance
    Serial.println("Ambulance détectée : " + String(ambulanceDetected));
  }
  Serial.println();
  // Ajouter ici le traitement des messages reçus
}

// Fonction pour connecter le client MQTT
bool connectMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);

  Serial.print("Connexion au broker MQTT... ");
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) { // Connexion sans utilisateur/mot de passe
      Serial.println("Connecté au broker MQTT.");
      return true;
    } else {
      Serial.print("Échec de connexion, code erreur: ");
      Serial.println(client.state());
      Serial.println("Nouvelle tentative dans 5 secondes...");
      delay(5000);
    }
  }
  return false;
}

// Fonction pour publier un message sur un topic
void publishMQTT(const char* topic, const char* message) {
  if (client.connected()) {
    client.publish(topic, message);
    // Serial.print("Message publié : ");
    //  Serial.println(message);
  } else {
    Serial.println("Impossible de publier, MQTT non connecté.");
  }
}

// Fonction pour s'abonner à un topic
void subscribeMQTT(const char* topic) {
  if (client.connected()) {
    client.subscribe(topic);
    Serial.print("Abonné au topic : ");
    Serial.println(topic);
  } else {
    Serial.println("Impossible de s'abonner, MQTT non connecté.");
  }
}

// Fonction pour maintenir la connexion MQTT
void handleMQTT() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop(); // Appel pour gérer les messages entrants et maintenir la connexion
}

#endif
