# 🚦 Feux de Circulation Intelligents avec Microservices et IoT

## Description du Projet

Ce projet vise à développer un **système intelligent de gestion des feux de circulation** basé sur des **microservices** pour analyser les flux de véhicules et optimiser la régulation du trafic. Il utilise des technologies avancées telles que **YOLOv8** pour la détection des véhicules et **MQTT** pour la communication IoT, avec une prise de décision optimisée par des outils d'IA.

Le système ajuste dynamiquement les feux de circulation en fonction de plusieurs critères tels que la **quantité de véhicules**, la **présence de piétons**, la **priorité des ambulances**, et les **données environnementales** (température, humidité, luminosité).

---

## 🛠️ **Architecture du Projet**

Le projet est constitué de **5 microservices** distincts, chacun ayant un rôle précis dans l'architecture globale du système :

![image](https://github.com/user-attachments/assets/c027e866-6dec-4306-95f9-bcc32b8e7363)

1. **Microservice 1 : IoT Sensor (ESP8266)**
   - Collecte de données environnementales (température, humidité, luminosité).
   - Communication via **MQTT** avec **Node-RED** pour envoyer les données collectées.
   - **Technologies** : ESP8266, MQTT, DHT11, Mosquitto.

2. **Microservice 2 : Node-RED**
   - Orchestration des données IoT et traitement des informations envoyées par les capteurs.
   - **Dashboard** Node-RED pour la visualisation des données et l’interaction avec les autres services.
   - **Technologies** : Node-RED, MQTT, HTTP.
<img width="959" alt="image" src="https://github.com/user-attachments/assets/1a82d6ad-bc67-4af1-ba42-ad062671d86b" />
<img width="959" alt="image" src="https://github.com/user-attachments/assets/61f89f21-36df-4489-8871-681a9321e275" />
<img width="955" alt="image" src="https://github.com/user-attachments/assets/adbfe4f8-c109-4a2c-9034-2c78732c9ae1" />
<img width="950" alt="image" src="https://github.com/user-attachments/assets/2e526146-ba7f-45b9-a1d7-9fe276a83d0d" />

3. **Microservice 3 : Détection des Véhicules**
   - Utilisation du modèle **YOLOv8** pour la détection en temps réel des véhicules, piétons et ambulances.
   - Publication des résultats via **MQTT** vers **ESP8266**.
   - **Technologies** : YOLOv8 (Ultralytics), Python.

  <img width="958" alt="image" src="https://github.com/user-attachments/assets/ebe4e428-b252-440d-8109-1c088d636701" />

4. **Microservice 4 : Frontend Flutter (Application Mobile)**
   - Affichage des informations des feux de circulation et de la localisation des véhicules via **OpenStreetMap**.
   - Intégration d’un chatbot pour interagir avec l'utilisateur et afficher les informations du trafic.
   - **Technologies** : Flutter, OpenStreetMap, API Flask.

5. **Microservice 5 : Assistant IA**
   - Cette partie implémente un assistant intelligent utilisant un modèle Llama2 pour répondre aux questions de l'utilisateur en se basant sur des données chargées à partir d'un fichier CSV. 
   - Utilisation de **Hugging Face** pour le traitement du langage naturel et **LangChain** pour la gestion des flux de données textuelles.
   - **Technologies** : Hugging Face, LangChain, FAISS, Flask, Llama2 (7b) 3.8GB.

<img width="959" alt="image" src="https://github.com/user-attachments/assets/af82c897-67be-461a-9113-68d3c47a2725" />
<img width="959" alt="image" src="https://github.com/user-attachments/assets/e7e75f72-89f7-4b04-8d7b-1175eb0b841d" />
<img width="959" alt="image" src="https://github.com/user-attachments/assets/75db0b59-616c-4640-b369-1ad520756845" />

---

## 🔗 **Flux de Données**

1. Les **capteurs IoT** (ESP8266) envoient les données via **MQTT** à **Node-RED**.
2. **YOLOv8** détecte les véhicules et piétons et publie les résultats via **MQTT**.
3. Les données sont envoyées à une **application Flutter** via une **API HTTP** pour affichage sur une carte **OpenStreetMap**.
4. Le serveur **Flask** reçoit et traite les requêtes IA pour prendre des décisions sur le contrôle des feux de circulation.
5. **Node-RED** affiche les informations sur le **Dashboard** pour la surveillance en temps réel.

---

## 🛠️ **Technologies Utilisées**

- **IoT** : ESP8266, MQTT, Mosquitto
- **Backend** : Flask, Python
- **IA** : Hugging Face, LangChain, FAISS, Llama2
- **Détection d'Objets** : YOLOv8 (Ultralytics)
- **Orchestration** : Node-RED
- **Frontend** : Flutter, OpenStreetMap

---

## ⚙️ **Installation et Exécution**

1. **ESP8266 (Capteurs IoT)** :
   - Configurez le code Arduino pour envoyer les données de capteurs via MQTT.
   - Assurez-vous qu'un **broker MQTT** (Mosquitto) est en fonctionnement.

2. **Node-RED** :
   - Installez Node-RED via npm :
     ```bash
     npm install -g node-red
     ```
   - Importez le flow Node-RED fourni (`flow.json`).

3. **Détection des Véhicules avec YOLOv8** :
   - Installez les dépendances nécessaires pour YOLOv8 :
     ```bash
     pip install -r requirements.txt
     ```
   - Exécutez le modèle YOLOv8 pour la détection des véhicules et piétons via une caméra ou vidéo.

4. **Backend Flask** :
   - Installez les dépendances Flask et LangChain :
     ```bash
     pip install -r requirements.txt
     ```
   - Lancez le serveur Flask avec :
     ```bash
     python app.py
     ```

5. **Frontend Flutter** :
   - Installez Flutter depuis [flutter.dev](https://flutter.dev/).
   - Configurez l'application Flutter pour interagir avec l'API Flask.

---

## 📊 **Résultats**

- **Feux de circulation dynamiques** : Ajustement en temps réel des feux en fonction du nombre de véhicules, piétons, et ambulances détectées.
- **Visualisation des données IoT** via un **Node-RED Dashboard**.
- **Optimisation du trafic** grâce à l'IA, permettant une régulation optimale basée sur les conditions environnementales et de circulation.

---


