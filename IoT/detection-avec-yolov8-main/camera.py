from ultralytics import YOLO
import cv2
import paho.mqtt.client as mqtt

# Charger le modèle YOLOv8 pré-entraîné
model = YOLO("yolov8n.pt")  # Utilise un modèle pré-entraîné sur COCO

# Configurer MQTT
MQTT_BROKER = "192.168.33.200"  # Adresse de votre ESP8266 ou broker
MQTT_PORT = 1883
MQTT_TOPIC_VEHICLES = "traffic/vehicles"  # Sujet pour le nombre total de véhicules
MQTT_TOPIC_AMBULANCES = "traffic/ambulances"  # Sujet pour le nombre d'ambulances
MQTT_TOPIC_PEDESTRIANS = "traffic/pedestrians"  # Sujet pour le nombre de piétons

# Initialisation du client MQTT
client = mqtt.Client()
client.connect(MQTT_BROKER, MQTT_PORT, 60)

# Dictionnaire pour mapper les classes détectées
COCO_CLASSES = {
    1: "bicycle",
    2: "car",
    3: "motorcycle",
    5: "bus",
    7: "truck",
    0: "person",  # Inclure les piétons
    18: "ambulance",  # Ajout de la classe ambulance (hypothétique, selon le modèle)
}

# Capture vidéo (remplacez par une vidéo ou une caméra)
cap = cv2.VideoCapture(0)  # Remplacez par un fichier vidéo si nécessaire

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Redimensionner pour YOLO si nécessaire
    resized_frame = cv2.resize(frame, (640, 480))

    # Faire l'inférence avec YOLOv8
    results = model(resized_frame)

    # Compter les différents types de véhicules, ambulances et piétons
    total_vehicles = 0
    num_ambulances = 0
    num_pedestrians = 0

    for result in results[0].boxes:
        cls_id = int(result.cls[0])  # ID de classe détectée
        if cls_id in COCO_CLASSES:
            label = COCO_CLASSES[cls_id]
            if label in ["car", "truck", "bus", "motorcycle", "bicycle"]:
                total_vehicles += 1
            elif label == "ambulance":
                num_ambulances += 1
            elif label == "person":
                num_pedestrians += 1

    # Publier les données via MQTT
    client.publish(MQTT_TOPIC_VEHICLES, total_vehicles)
    client.publish(MQTT_TOPIC_AMBULANCES, num_ambulances)
    client.publish(MQTT_TOPIC_PEDESTRIANS, num_pedestrians)

    # Afficher les résultats sur l'image
    y_offset = 30  # Position verticale initiale pour afficher le texte

    # Afficher le nombre de piétons détectés
    cv2.putText(frame, f"Pedestrians: {num_pedestrians}", (10, y_offset), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    y_offset += 40  # Décalage vertical

    # Afficher le nombre total de véhicules détectés
    cv2.putText(frame, f"Total Vehicles: {total_vehicles}", (10, y_offset), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    y_offset += 40  # Décalage vertical

    # Afficher le nombre d'ambulances détectées
    cv2.putText(frame, f"Ambulances: {num_ambulances}", (10, y_offset), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    # Afficher la vidéo avec les annotations
    cv2.imshow("Traffic Monitor", frame)

    # Quitter avec 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
