#include <DHT.h>


#ifndef DHTControl
#define DHTControl


#define DHTPIN 15  // Pin du capteur DHT11
#define DHTTYPE DHT11  // Type de capteur DHT (DHT11 dans ce cas)

DHT dht(DHTPIN, DHTTYPE);  // Créer une instance de l'objet DHT

// Initialisation du capteur DHT
void initDHT() {
  dht.begin();
}

// Lecture de la température
float readTemperature() {
  float temperature = dht.readTemperature();  // Lire la température en °C
  if (isnan(temperature)) {  // Vérifier si la lecture a échoué
    Serial.println("Erreur de lecture du capteur DHT11 (Température)");
    return 0;  // Retourner -1 en cas d'erreur
  }
  return temperature;
}

// Lecture de l'humidité
float readHumidity() {
  float humidity = dht.readHumidity();  // Lire l'humidité en pourcentage
  if (isnan(humidity)) {  // Vérifier si la lecture a échoué
    Serial.println("Erreur de lecture du capteur DHT11 (Humidité)");
    return 0;  // Retourner -1 en cas d'erreur
  }
  return humidity;
}

#endif
