# README - Détection avec la Caméra (YOLOv8)

## Description
Ce projet permet de réaliser une détection d'objets en temps réel à partir d'une vidéo ou d'une caméra. Il utilise le modèle YOLOv8 pour effectuer la détection et dessiner des annotations sur les frames affichées.

---

## Fonctionnalités
- Détection d'objets en temps réel à partir d'une vidéo ou d'une caméra.
- Annotation des objets détectés (classes, boîtes englobantes, scores de confiance).
- Utilisation du modèle pré-entraîné YOLOv8 pour une détection rapide et efficace.

---

## Prérequis

### Bibliothèques Python nécessaires :
Assurez-vous d'avoir les bibliothèques suivantes installées sur votre système :

- **ultralytics** (pour YOLOv8)
- **opencv-python** (pour le traitement des frames de vidéo)
- **numpy** (pour la manipulation des matrices)

Vous pouvez les installer avec la commande suivante :
```bash
pip install ultralytics opencv-python numpy
```

### Ressources nécessaires :
- Vidéo (fichier « video.mp4 » dans le même dossier que le script).
- Modèle YOLOv8 pré-entraîné (ex: `yolov8s.pt`).
  
Vous pouvez télécharger le modèle YOLOv8 à partir de [Ultralytics](https://ultralytics.com/).

---

## Utilisation

### 1. Configuration du script
Assurez-vous que les fichiers suivants sont présents dans le même dossier que le script Python :
- `video.mp4` : fichier vidéo sur lequel effectuer la détection.
- `yolov8s.pt` : fichier du modèle YOLOv8 pré-entraîné.

### 2. Lancer la détection à partir d'une vidéo
Exécutez le script avec Python :
```bash
python script.py
```
Le script ouvrira une fenêtre où les objets détectés seront annotés sur chaque frame. Appuyez sur `q` pour fermer la fenêtre.

### 3. Modification pour une détection via caméra
Si vous souhaitez utiliser la caméra au lieu d'une vidéo, remplacez la fonction `detect_from_video(video_path)` par la suivante dans le script :

```python
def detect_from_camera():
    cap = cv2.VideoCapture(0)  # 0 pour la caméra par défaut
    if not cap.isOpened():
        print("Erreur : Impossible d'accéder à la caméra.")
        return

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            print("Erreur lors de la capture de la frame.")
            break

        frame = cv2.resize(frame, (640, 384))
        frame = np.ascontiguousarray(frame, dtype=np.uint8)
        results = model(frame, conf=0.25)

        for obj in results[0].boxes:
            x1, y1, x2, y2 = map(int, obj.xyxy[0])
            cls = model.names[int(obj.cls[0])]
            conf = obj.conf[0]
            label = f"{cls} {conf:.2f}"
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

        cv2.imshow('Détection en temps réel', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

# Lancer la détection avec la caméra
if __name__ == "__main__":
    detect_from_camera()
```
---

## Structure du Projet
```
project-directory/
├── script.py          # Script Python principal
├── video.mp4          # Vidéo pour la détection
├── yolov8s.pt         # Modèle YOLOv8 pré-entraîné
```

---

## Raccourcis Clés
- `q` : Quitter la fenêtre de détection en temps réel.

---

## Améliorations Potentielles
- Ajouter une fonctionnalité pour enregistrer la vidéo annotée.
- Intégrer des modules pour envoyer les résultats de détection à une base de données ou une interface web.
- Optimiser le script pour des performances temps réel sur des systèmes moins puissants.

---

## Ressources
- Documentation YOLOv8 : [Ultralytics YOLO](https://ultralytics.com/)
- OpenCV : [Site Officiel](https://opencv.org/)
- NumPy : [Documentation NumPy](https://numpy.org/)

---

## Licence
Ce projet est sous licence MIT. Vous êtes libre de l'utiliser, le modifier et le distribuer.
