# 🚦 Feux de Circulation Intelligents avec Microservices

## Description du Projet

Ce projet vise à développer un système **intelligent de gestion des feux de circulation** basé sur des microservices pour analyser les flux de véhicules et optimiser la régulation du trafic. Il utilise plusieurs technologies modernes comme **YOLOv8** pour la détection des véhicules, **MQTT** pour la communication IoT, et des outils d'IA pour la prise de décision intelligente.

---

## 🛠️ **Architecture du Projet**

Le projet utilise **5 microservices** distincts pour assurer une architecture modulaire et performante :

1. **Microservice 1 : IoT Sensor (ESP8266)**
   - Capteurs connectés pour la collecte des données environnementales (humidité, température) et luminosité.
   - Communication via le protocole **MQTT** vers **Node-RED**.
   - **Technologies** : ESP8266, MQTT, DHT11, Mosquitto.

2. **Microservice 2 : Node-RED**
   - Orchestration des données IoT et traitement des informations envoyées par les capteurs.
   - Interface utilisateur via le **Node-RED Dashboard** et **Worldmap**.
   - **Technologies** : Node-RED, MQTT, HTTP.

3. **Microservice 3 : Détection des Véhicules**
   - Détection en temps réel des véhicules et des piétons via **YOLOv8**.
   - Transmission des résultats via API **HTTP** vers **Flutter** pour affichage sur une carte.
   - **Technologies** : Ultralytics YOLOv8, Python.

4. **Microservice 4 : Frontend Flutter (Mobile App)**
   - Affichage de l’état des feux, des statistiques de trafic, et localisation des véhicules via **OpenStreetMap**.
   - Intégration d’un chatbot pour les requêtes utilisateurs.
   - **Technologies** : Flutter, OpenStreetMap, API Flask.

5. **Microservice 5 : IA et Prise de Décision**
   - Utilisation de modèles **Hugging Face** pour l’analyse des données textuelles.
   - Gestion des requêtes via **LangChain** et stockage des vecteurs avec **FAISS**.
   - Serveur backend utilisant **Flask**.
   - **Technologies** : Hugging Face, LangChain, FAISS, Flask.

---

## 🔗 **Flux de Données**

1. Les capteurs IoT transmettent les données via **MQTT** → **Node-RED**.
2. **YOLOv8** détecte les véhicules et piétons → transmet les résultats via MQTT.
3. Les résultats sont affichés sur une carte **OpenStreetMap** via l'application **Flutter**.
4. Le serveur backend avec **Flask** gère les décisions IA grâce aux modèles de **Hugging Face** et **FAISS**.
5. **Node-RED Dashboard** offre une visualisation des données IoT et de la situation du trafic.

---

## 🛠️ **Technologies Utilisées**

- **IoT** : ESP8266, MQTT, Mosquitto
- **Backend** : Flask, Python
- **IA** : Hugging Face, LangChain, FAISS,Llama2
- **Détection d'Objets** : YOLOv8 (Ultralytics)
- **Orchestration** : Node-RED
- **Frontend** : Flutter, OpenStreetMap

---

## ⚙️ **Installation et Exécution**

1. **ESP8266** :
   - Configurez le code Arduino pour la communication MQTT.
   - Assurez-vous d’avoir un broker MQTT (Mosquitto) en fonctionnement.

2. **Node-RED** :
   - Installez Node-RED via npm :
     ```bash
     npm install -g node-red
     ```
   - Importez le flow Node-RED fourni (`flow.json`).

3. **Détection des Véhicules (YOLOv8)** :
   - Installez Ultralytics :
     ```bash
     pip install ultralytics
     ```
   - Exécutez le modèle YOLOv8 avec votre flux vidéo.

4. **Backend Flask** :
   - Installez les dépendances :
     ```bash
     pip install flask langchain faiss-cpu transformers
     ```
   - Lancez le serveur Flask :
     ```bash
     python app.py
     ```

5. **Frontend Flutter** :
   - Installez Flutter depuis [flutter.dev](https://flutter.dev/).
   - Connectez l’application à l’API Flask.

---

## 📊 **Résultats Attendus**

- Régulation dynamique des feux en fonction du trafic détecté.
- Visualisation des données IoT et trafic via des dashboards interactifs.
- Communication fluide entre les microservices pour une optimisation en temps réel.

---


