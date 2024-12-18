#include "WiFi.h"
#include "MQTT.h"
#include "firebase.h"
#include "ledControl.h"
#include "DHTControl.h"
#include "photocell.h"


unsigned long lastUpdateTime = 0;
unsigned long currentDelay = 5000;  // Délai par défaut en millisecondes
bool inverted = false;
int currentState = 0;
bool buttonState = HIGH;
const float TEMP_THRESHOLD = 1.0;  // Seuil de changement en °C
const float HUM_THRESHOLD = 5.0;   // Seuil de changement en % d'humidité
int ldrValue = 0;
int brightness = 0;

float currentTemperature = 0.0;
float currentHumidity = 0.0;
float lastTemperature = 0.0;
float lastHumidity = 0.0;

unsigned long lastFirebaseUpdate = 0;         // Temps du dernier envoi
const unsigned long updateInterval = 300000;  // 5 minutes en millisecondes

const int LDR_THRESHOLD_NIGHT = 500;  // Seuil pour la nuit
const int LDR_THRESHOLD_DAY = 800;    // Seuil pour le jour
// Variables pour stocker l'état précédent des feux
int prevRedLane1 = LOW, prevYellowLane1 = LOW, prevGreenLane1 = LOW;
int prevRedLane2 = LOW, prevYellowLane2 = LOW, prevGreenLane2 = LOW;


void setup() {
  Serial.begin(115200);  // Initialisation de la communication série
  connectWiFi();         // Appel de la fonction pour se connecter au Wi-Fi
  initFirebase();        // Initialisation de Firebase

  // Connexion au broker MQTT
  if (connectMQTT()) {
    subscribeMQTT("traffic/vehicles");
    subscribeMQTT("traffic/pedestrians");
    subscribeMQTT("traffic/ambulances");
    subscribeMQTT("trafficLight/toggle");
  }

  initDHT();  // Initialiser le capteur DHT

  // Initialiser les broches pour les feux de circulation
  pinMode(RED_LIGHT_LANE1, OUTPUT);
  pinMode(YELLOW_LIGHT_LANE1, OUTPUT);
  pinMode(GREEN_LIGHT_LANE1, OUTPUT);
  pinMode(RED_LIGHT_LANE2, OUTPUT);
  pinMode(YELLOW_LIGHT_LANE2, OUTPUT);
  pinMode(GREEN_LIGHT_LANE2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Initialiser le bouton d'urgence
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Utilisation de la résistance de pull-up interne pour le bouton
}

void loop() {

  // Gestion du bouton pour inversion de cycle
  bool currentButtonState = digitalRead(BUTTON_PIN);
  if (currentButtonState == LOW && buttonState == HIGH) {
    inverted = !inverted;
    setLights();
    delay(50);
  }
  buttonState = currentButtonState;

  // Changement automatique en fonction de l'état des feux
  if (millis() - lastUpdateTime >= currentDelay) {
    currentState = (currentState + 1) % 4;
    setLights();
    lastUpdateTime = millis();
    delay(50);
  }


  // Publier un message sur le topic toutes les 5 secondes
  static unsigned long lastPublish = 0;


  // Gérer MQTT (messages entrants, maintien de la connexion)
  handleMQTT();

  // Contrôle DHT
  // Lire les valeurs de température et d'humidité
  currentTemperature = readTemperature();  // Lire la température
  currentHumidity = readHumidity();        // Lire l'humidité

  // Publier les messages MQTT
  publishMQTT("environment/temperature", String(currentTemperature).c_str());  // Publier la température
  publishMQTT("environment/humidity", String(currentHumidity).c_str());        // Publier l'humidité



  // Vérifier si la température ou l'humidité a changé significativement
  if (abs(currentTemperature - lastTemperature) >= TEMP_THRESHOLD || abs(currentHumidity - lastHumidity) >= HUM_THRESHOLD) {
    //publishEnvironmentalData(currentTemperature, currentHumidity, numVehicles, numPedestrians, ambulanceDetected);
    lastTemperature = currentTemperature;
    lastHumidity = currentHumidity;

    // Afficher et publier les mesures
    Serial.print("Température mise à jour : ");
    Serial.println(currentTemperature);
    Serial.print("Humidité mise à jour : ");
    Serial.println(currentHumidity);
  }
  if (millis() - lastFirebaseUpdate >= updateInterval) {
    lastFirebaseUpdate = millis();

  }
    publishEnvironmentalData(currentTemperature, currentHumidity, numVehicles, numPedestrians, ambulanceDetected);

  // Ajuster le délai basé sur les conditions
   adjustDelayBasedOnConditions(currentTemperature, currentHumidity, numVehicles, numPedestrians, ambulanceDetected);

  // Ajuster la luminosité des LED en fonction du LDR
  adjustLEDsBasedOnLDR();

  publishDelayToMQTT(currentDelay);


  // Lire l'état actuel des feux
  int currentRedLane1 = digitalRead(RED_LIGHT_LANE1);
  int currentYellowLane1 = digitalRead(YELLOW_LIGHT_LANE1);
  int currentGreenLane1 = digitalRead(GREEN_LIGHT_LANE1);

  int currentRedLane2 = digitalRead(RED_LIGHT_LANE2);
  int currentYellowLane2 = digitalRead(YELLOW_LIGHT_LANE2);
  int currentGreenLane2 = digitalRead(GREEN_LIGHT_LANE2);

  // Vérifier si un changement est détecté dans les feux de voie 1
  if (currentRedLane1 != prevRedLane1 || currentYellowLane1 != prevYellowLane1 || currentGreenLane1 != prevGreenLane1 || currentRedLane2 != prevRedLane2 || currentYellowLane2 != prevYellowLane2 || currentGreenLane2 != prevGreenLane2) {
    // Publier l'état des feux
    publishLightsStatus();

    // Mettre à jour les états précédents
    prevRedLane1 = currentRedLane1;
    prevYellowLane1 = currentYellowLane1;
    prevGreenLane1 = currentGreenLane1;

    prevRedLane2 = currentRedLane2;
    prevYellowLane2 = currentYellowLane2;
    prevGreenLane2 = currentGreenLane2;
  }
}


void adjustDelayBasedOnConditions(float temperature, float humidity, int numVehicles, int numPedestrians, bool ambulanceDetected) {
  String conditionMessage = ""; // Message descriptif

  // Vérification des conditions météo et ajustement du délai
  if (temperature < 20 && humidity > 60) {
    conditionMessage = "Météo : Froid avec humidité élevée.";
    if (ambulanceDetected) {
      currentDelay = 2000; // Priorité à l'ambulance
      conditionMessage += " Ambulance détectée. Délai : 2000 ms.";
    } else if (numPedestrians > 10) {
      currentDelay = 7000; // Priorité aux piétons
      conditionMessage += " Beaucoup de piétons. Délai ajusté à 7000 ms.";
    } else if (numVehicles > 20) {
      currentDelay = 4000; // Ajustement pour un trafic fluide
      conditionMessage += " Trafic dense. Délai ajusté à 4000 ms.";
    } else {
      currentDelay = 5000; // Délai par défaut
      conditionMessage += " Conditions normales. Délai ajusté à 5000 ms.";
    }
  } else if (temperature >= 20 && temperature <= 30 && humidity <= 60) {
    conditionMessage = "Météo : Température modérée et humidité normale.";
    if (ambulanceDetected) {
      currentDelay = 2000;
      conditionMessage += " Ambulance détectée. Délai : 2000 ms.";
    } else if (numPedestrians > 10) {
      currentDelay = 6000;
      conditionMessage += " Beaucoup de piétons. Délai ajusté à 6000 ms.";
    } else if (numVehicles > 30) {
      currentDelay = 3000;
      conditionMessage += " Trafic dense. Délai ajusté à 3000 ms.";
    } else {
      currentDelay = 4000;
      conditionMessage += " Conditions normales. Délai ajusté à 4000 ms.";
    }
  } else if (temperature > 30 || humidity < 30) {
    conditionMessage = "Météo : Température élevée ou faible humidité.";
    if (ambulanceDetected) {
      currentDelay = 2000;
      conditionMessage += " Ambulance détectée. Délai : 2000 ms.";
    } else if (numPedestrians > 10) {
      currentDelay = 8000;
      conditionMessage += " Beaucoup de piétons. Délai ajusté à 8000 ms.";
    } else if (numVehicles > 10) {
      currentDelay = 3000;
      conditionMessage += " Trafic dense. Délai ajusté à 3000 ms.";
    } else {
      currentDelay = 4000;
      conditionMessage += " Conditions normales. Délai ajusté à 4000 ms.";
    }
  } else {
    conditionMessage = "Météo : Conditions non spécifiques.";
    if (ambulanceDetected) {
      currentDelay = 2000;
      conditionMessage += " Ambulance détectée. Délai : 2000 ms.";
    } else if (numPedestrians > 5) {
      currentDelay = 5000;
      conditionMessage += " Piétons détectés. Délai ajusté à 5000 ms.";
    } else if (numVehicles > 15) {
      currentDelay = 3500;
      conditionMessage += " Trafic dense. Délai ajusté à 3500 ms.";
    } else {
      currentDelay = 4000;
      conditionMessage += " Conditions normales. Délai ajusté à 4000 ms.";
    }
  }

  // Afficher les informations sur le moniteur série
  Serial.println(conditionMessage);

  // Créer un message JSON avec toutes les informations
  String message = "{\"temperature\": " + String(temperature, 2) +
                   ", \"humidity\": " + String(humidity, 2) +
                   ", \"numVehicles\": " + String(numVehicles) +
                   ", \"numPedestrians\": " + String(numPedestrians) +
                   ", \"ambulanceDetected\": " + (ambulanceDetected ? "true" : "false") +
                   ", \"delay\": " + String(currentDelay) +
                   ", \"conditionMessage\": \"" + conditionMessage + "\"}";

  // Publier le message sur un topic MQTT
  publishMQTT("traffic/conditions", message.c_str());
}

void publishEnvironmentalData(float temperature, float humidity, int numVehicles, int numPedestrians, bool ambulanceDetected) {
  // Créer un objet JSON avec un identifiant unique basé sur le timestamp
  unsigned long timestamp = millis();  // Utiliser millis() comme identifiant unique basé sur le temps
  unsigned long epochTime = millis() / 1000;  // Convertir en secondes depuis 1970 pour avoir un timestamp UTC

  // Créer la date au format lisible (jour/mois/année)
  String dateString = getFormattedDate(epochTime);  // Supposons que tu as une fonction qui formate les dates

  // Définir le chemin Firebase avec le timestamp
  String path = "/feux/data/" + String(epochTime);  // Utiliser l'epoch time comme identifiant unique dans le chemin

  // Créer l'objet JSON pour stocker toutes les informations
String message = "{\"temperature\": " + String(temperature, 2) +
                 ", \"humidity\": " + String(humidity, 2) +
                 ", \"numVehicles\": " + String(numVehicles) +
                 ", \"numPedestrians\": " + String(numPedestrians) +
                 ", \"ambulanceDetected\": " + (ambulanceDetected ? "true" : "false") +
                  ", \"delay_time\": " + String(currentDelay) + "}"; // Correction : ajouter une virgule avant la fin de la chaîne

// Publier les données sur le topic MQTT 'feux_de_circulation/env'
publishMQTT("feux_de_circulation/env", message.c_str());

  // Envoyer les données à Firebase sous un chemin unique basé sur l'epoch time
  bool success = true;  // Variable pour suivre si toutes les opérations Firebase réussissent
  success &= Firebase.setString(firebaseData, path + "/temperature", String(temperature, 2));
  success &= Firebase.setString(firebaseData, path + "/humidity", String(humidity, 2));
  success &= Firebase.setInt(firebaseData, path + "/numVehicles", numVehicles);
  success &= Firebase.setInt(firebaseData, path + "/numPedestrians", numPedestrians);
  success &= Firebase.setBool(firebaseData, path + "/ambulanceDetected", ambulanceDetected);
  success &= Firebase.setString(firebaseData, path + "/delay_time", String(currentDelay));
  success &= Firebase.setString(firebaseData, path + "/date", dateString);  // Ajout de la date formatée

  // Afficher un seul message dans le Serial Monitor
  if (success) {
    Serial.println("Données envoyées à Firebase avec succès :");
    Serial.println(message);  // Afficher toutes les informations envoyées
  } else {
    Serial.println("Erreur lors de l'envoi des données à Firebase : " + firebaseData.errorReason());
  }
}

// Fonction pour obtenir la date au format "YYYY-MM-DD"
String getFormattedDate(unsigned long epochTime) {
  // Utiliser une bibliothèque de gestion de temps comme "TimeLib" pour formater l'epoch time en une date lisible.
  time_t t = epochTime;  // Convertir le timestamp en time_t
  struct tm* tm_info = localtime(&t);  // Convertir en structure tm pour manipuler la date
  char buffer[11];
  strftime(buffer, 11, "%Y-%m-%d", tm_info);  // Format "YYYY-MM-DD"
  return String(buffer);
}


void adjustLEDsBasedOnLDR() {
  ldrValue = analogRead(LIGHT_SENSOR_PIN);
  brightness = map(ldrValue, LDR_THRESHOLD_NIGHT, LDR_THRESHOLD_DAY, 255, 0);
  brightness = constrain(brightness, 50, 255);
  Serial.print("Luminosité détectée : ");
  Serial.print(brightness);
  Serial.print(" | Valeur LDR : ");
  Serial.println(ldrValue);

  if (currentState == 0) {  // Green Lane 1, Red Lane 2
    analogWrite(GREEN_LIGHT_LANE1, brightness);
    analogWrite(RED_LIGHT_LANE2, brightness);
  } else if (currentState == 1) {  // Yellow Lane 1, Red Lane 2
    analogWrite(YELLOW_LIGHT_LANE1, brightness);
    analogWrite(RED_LIGHT_LANE2, brightness);
  } else if (currentState == 2) {  // Red Lane 1, Green Lane 2
    analogWrite(RED_LIGHT_LANE1, brightness);
    analogWrite(GREEN_LIGHT_LANE2, brightness);
  } else {  // Red Lane 1, Yellow Lane 2
    analogWrite(RED_LIGHT_LANE1, brightness);
    analogWrite(YELLOW_LIGHT_LANE2, brightness);
  }

  // Publish brightness and LDR value to an MQTT topic

  publishMQTT("traffic/light/brightness", String(brightness).c_str());  // Publish brightness to topic

  publishMQTT("traffic/light/ldrValue", String(ldrValue).c_str());  // Publish LDR value to topic


  // Adjust LED brightness based on the current state
  if (currentState == 0) {  // Green Lane 1, Red Lane 2
    analogWrite(GREEN_LIGHT_LANE1, brightness);
    analogWrite(RED_LIGHT_LANE2, brightness);
  } else if (currentState == 1) {  // Yellow Lane 1, Red Lane 2
    analogWrite(YELLOW_LIGHT_LANE1, brightness);
    analogWrite(RED_LIGHT_LANE2, brightness);
  } else if (currentState == 2) {  // Red Lane 1, Green Lane 2
    analogWrite(RED_LIGHT_LANE1, brightness);
    analogWrite(GREEN_LIGHT_LANE2, brightness);
  } else {  // Red Lane 1, Yellow Lane 2
    analogWrite(RED_LIGHT_LANE1, brightness);
    analogWrite(YELLOW_LIGHT_LANE2, brightness);
  }
}

void publishLightsStatus() {
  // Pour voie 1
  String statusLane1;
  if (digitalRead(RED_LIGHT_LANE1)) {
    statusLane1 = "red";
  } else if (digitalRead(GREEN_LIGHT_LANE1)) {
    statusLane1 = "green";
  } else if (digitalRead(YELLOW_LIGHT_LANE1)) {
    statusLane1 = "yellow";
  } else {
    statusLane1 = "off";
  }

  String messageLane1 = "{\"status\": \"" + statusLane1 + "\"}";
  publishMQTT("feux/intersection1/lane1", messageLane1.c_str());

  // Pour voie 2
  String statusLane2;
  if (digitalRead(RED_LIGHT_LANE2)) {
    statusLane2 = "red";
  } else if (digitalRead(GREEN_LIGHT_LANE2)) {
    statusLane2 = "green";
  } else if (digitalRead(YELLOW_LIGHT_LANE2)) {
    statusLane2 = "yellow";
  } else {
    statusLane2 = "off";
  }

  String messageLane2 = "{\"status\": \"" + statusLane2 + "\"}";
  publishMQTT("feux/intersection1/lane2", messageLane2.c_str());
}

void setLights() {
  // Réinitialiser tous les feux
  digitalWrite(RED_LIGHT_LANE1, LOW);  // Feu rouge lane 1 OFF
  digitalWrite(YELLOW_LIGHT_LANE1, LOW); // Feu jaune lane 1 OFF
  digitalWrite(GREEN_LIGHT_LANE1, LOW);  // Feu vert lane 1 OFF
  digitalWrite(RED_LIGHT_LANE2, LOW);   // Feu rouge lane 2 OFF
  digitalWrite(YELLOW_LIGHT_LANE2, LOW); // Feu jaune lane 2 OFF
  digitalWrite(GREEN_LIGHT_LANE2, LOW);  // Feu vert lane 2 OFF

  if (!inverted) {  // Cas où l'inversion n'est pas activée
    switch (currentState) {
      case 0:
        // Feu vert lane 1 et Feu rouge lane 2 allumés
        digitalWrite(GREEN_LIGHT_LANE1, HIGH);  // Feu vert lane 1 ON
        digitalWrite(RED_LIGHT_LANE2, HIGH);   // Feu rouge lane 2 ON
        // Éteindre les autres feux non concernés
        digitalWrite(YELLOW_LIGHT_LANE1, LOW); // Feu jaune lane 1 OFF
        digitalWrite(GREEN_LIGHT_LANE2, LOW);  // Feu vert lane 2 OFF
        digitalWrite(YELLOW_LIGHT_LANE2, LOW); // Feu jaune lane 2 OFF
        digitalWrite(RED_LIGHT_LANE1, LOW);    // Feu rouge lane 1 OFF
        break;
      case 1:
        // Feu jaune lane 1 et Feu rouge lane 2 allumés
        digitalWrite(YELLOW_LIGHT_LANE1, HIGH); // Feu jaune lane 1 ON
        digitalWrite(RED_LIGHT_LANE2, HIGH);    // Feu rouge lane 2 ON
        // Éteindre les autres feux non concernés
        digitalWrite(GREEN_LIGHT_LANE1, LOW);  // Feu vert lane 1 OFF
        digitalWrite(GREEN_LIGHT_LANE2, LOW);  // Feu vert lane 2 OFF
        digitalWrite(YELLOW_LIGHT_LANE2, LOW); // Feu jaune lane 2 OFF
        digitalWrite(RED_LIGHT_LANE1, LOW);    // Feu rouge lane 1 OFF
        break;
      case 2:
        // Feu rouge lane 1 et Feu vert lane 2 allumés
        digitalWrite(RED_LIGHT_LANE1, HIGH);    // Feu rouge lane 1 ON
        digitalWrite(GREEN_LIGHT_LANE2, HIGH);  // Feu vert lane 2 ON
        // Éteindre les autres feux non concernés
        digitalWrite(YELLOW_LIGHT_LANE1, LOW); // Feu jaune lane 1 OFF
        digitalWrite(RED_LIGHT_LANE2, LOW);    // Feu rouge lane 2 OFF
        digitalWrite(YELLOW_LIGHT_LANE2, LOW); // Feu jaune lane 2 OFF
        break;
      case 3:
        // Feu rouge lane 1 et Feu jaune lane 2 allumés
        digitalWrite(RED_LIGHT_LANE1, HIGH);    // Feu rouge lane 1 ON
        digitalWrite(YELLOW_LIGHT_LANE2, HIGH); // Feu jaune lane 2 ON
        // Éteindre les autres feux non concernés
        digitalWrite(GREEN_LIGHT_LANE1, LOW);  // Feu vert lane 1 OFF
        digitalWrite(RED_LIGHT_LANE2, LOW);    // Feu rouge lane 2 OFF
        digitalWrite(GREEN_LIGHT_LANE2, LOW);  // Feu vert lane 2 OFF
        break;
    }
  } else {  // Cas où l'inversion est activée
    switch (currentState) {
      case 0:
        // Feu vert lane 2 et Feu rouge lane 1 allumés
        digitalWrite(GREEN_LIGHT_LANE2, HIGH);  // Feu vert lane 2 ON
        digitalWrite(RED_LIGHT_LANE1, HIGH);    // Feu rouge lane 1 ON
        // Éteindre les autres feux non concernés
        digitalWrite(YELLOW_LIGHT_LANE1, LOW); // Feu jaune lane 1 OFF
        digitalWrite(GREEN_LIGHT_LANE1, LOW);  // Feu vert lane 1 OFF
        digitalWrite(YELLOW_LIGHT_LANE2, LOW); // Feu jaune lane 2 OFF
        break;
      case 1:
        // Feu jaune lane 2 et Feu rouge lane 1 allumés
        digitalWrite(YELLOW_LIGHT_LANE2, HIGH); // Feu jaune lane 2 ON
        digitalWrite(RED_LIGHT_LANE1, HIGH);    // Feu rouge lane 1 ON
        // Éteindre les autres feux non concernés
        digitalWrite(GREEN_LIGHT_LANE1, LOW);  // Feu vert lane 1 OFF
        digitalWrite(GREEN_LIGHT_LANE2, LOW);  // Feu vert lane 2 OFF
        digitalWrite(YELLOW_LIGHT_LANE1, LOW); // Feu jaune lane 1 OFF
        break;
      case 2:
        // Feu rouge lane 2 et Feu vert lane 1 allumés
        digitalWrite(RED_LIGHT_LANE2, HIGH);    // Feu rouge lane 2 ON
        digitalWrite(GREEN_LIGHT_LANE1, HIGH);  // Feu vert lane 1 ON
        // Éteindre les autres feux non concernés
        digitalWrite(YELLOW_LIGHT_LANE1, LOW); // Feu jaune lane 1 OFF
        digitalWrite(RED_LIGHT_LANE1, LOW);    // Feu rouge lane 1 OFF
        digitalWrite(YELLOW_LIGHT_LANE2, LOW); // Feu jaune lane 2 OFF
        break;
      case 3:
        // Feu rouge lane 2 et Feu jaune lane 1 allumés
        digitalWrite(RED_LIGHT_LANE2, HIGH);    // Feu rouge lane 2 ON
        digitalWrite(YELLOW_LIGHT_LANE1, HIGH); // Feu jaune lane 1 ON
        // Éteindre les autres feux non concernés
        digitalWrite(GREEN_LIGHT_LANE1, LOW);  // Feu vert lane 1 OFF
        digitalWrite(RED_LIGHT_LANE1, LOW);    // Feu rouge lane 1 OFF
        digitalWrite(GREEN_LIGHT_LANE2, LOW);  // Feu vert lane 2 OFF
        break;
    }
  }
}



void publishDelayToMQTT(int delay) {
  String topic = "traffic/delay";  // Le topic pour le délai
  String message = String(delay);  // Convertir le délai en chaîne de caractères

  // Publier le message sur le topic
  publishMQTT(topic.c_str(), message.c_str());
}
