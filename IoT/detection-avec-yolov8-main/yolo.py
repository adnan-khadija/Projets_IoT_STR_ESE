from ultralytics import YOLO
import cv2
import paho.mqtt.client as mqtt
import time

# Charger le modèle YOLOv8 pré-entraîné
model = YOLO("yolov8n.pt")  # Utilise un modèle pré-entraîné sur COCO

# Configurer MQTT
MQTT_BROKER = "192.168.26.200"  # Adresse de votre broker
MQTT_PORT = 1883
MQTT_TOPIC_VEHICLES = "traffic/vehicles"
MQTT_TOPIC_AMBULANCES = "traffic/ambulances"
MQTT_TOPIC_PEDESTRIANS = "traffic/pedestrians"

# Initialisation du client MQTT
client = mqtt.Client()
client.connect(MQTT_BROKER, MQTT_PORT, 60)

# Classes COCO
COCO_CLASSES = {
    1: "bicycle",
    2: "car",
    3: "motorcycle",
    5: "bus",
    7: "truck",
    0: "person",
    18: "ambulance",  # Hypothétique (vérifiez si votre modèle supporte cette classe)
}

# Capture vidéo
cap = cv2.VideoCapture("video2.mp4")  # Remplacez par votre fichier vidéo
if not cap.isOpened():
    print("Erreur lors de l'ouverture de la vidéo.")
    exit()

# Variables pour compter les objets
total_vehicles_in_10_sec = 0  # Compteur pour le total de véhicules sur 10 secondes
num_ambulances = 0
num_pedestrians = 0

# Délai pour réinitialiser les compteurs
start_time = time.time()

while True:
    ret, frame = cap.read()
    if not ret:
        print("Fin de la vidéo. Redémarrage.")
        cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
        continue

    # Faire l'inférence YOLO
    results = model(frame)

    # Réinitialiser les compteurs à chaque itération
    vehicles_in_current_period = 0
    num_ambulances = 0
    num_pedestrians = 0

    # Parcourir les résultats de YOLO
    for result in results[0].boxes:
        cls_id = int(result.cls[0])  # ID de la classe
        label = COCO_CLASSES.get(cls_id, "unknown")
        x1, y1, x2, y2 = map(int, result.xyxy[0])  # Coordonnées du rectangle

        # Compter les véhicules détectés (qu'ils soient en mouvement ou non)
        if label in ["car", "truck", "bus", "motorcycle", "bicycle"]:
            vehicles_in_current_period += 1  # Compter les véhicules pendant cette période
            color = (255, 0, 0)  # Bleu pour les véhicules
        elif label == "ambulance":
            num_ambulances += 1
            color = (0, 0, 255)  # Rouge pour les ambulances
        elif label == "person":
            num_pedestrians += 1
            color = (0, 255, 0)  # Vert pour les piétons
        else:
            continue

        # Dessiner un rectangle autour de l'objet
        cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)
        cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

    # Afficher les résultats sur l'image
    cv2.putText(frame, f"Vehicles Detected in 10s: {vehicles_in_current_period}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)
    cv2.putText(frame, f"Ambulances: {num_ambulances}", (10, 70), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)
    cv2.putText(frame, f"Pedestrians: {num_pedestrians}", (10, 110), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

    # Afficher la vidéo avec les annotations
    cv2.namedWindow("Traffic Monitor", cv2.WINDOW_NORMAL)
    cv2.imshow("Traffic Monitor", frame)

    # Calculer les résultats toutes les 10 secondes
    elapsed_time = time.time() - start_time
    if elapsed_time > 10:  # Après 10 secondes
        # Publier les résultats via MQTT
        client.publish(MQTT_TOPIC_VEHICLES, vehicles_in_current_period)
        client.publish(MQTT_TOPIC_AMBULANCES, num_ambulances)
        client.publish(MQTT_TOPIC_PEDESTRIANS, num_pedestrians)
        print(f"Publié via MQTT : Vehicles={vehicles_in_current_period}, Ambulances={num_ambulances}, Pedestrians={num_pedestrians}")
        
        # Réinitialiser les variables pour la prochaine période de 10 secondes
        start_time = time.time()  # Réinitialiser le temps pour le prochain intervalle de 10 secondes
        total_vehicles_in_10_sec = 0  # Réinitialiser le compteur total des véhicules dans les 10 secondes

    # Quitter avec 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
