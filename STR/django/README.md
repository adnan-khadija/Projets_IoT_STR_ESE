# Simulateur d'Ordonnancement de Tâches

Application Django pour simuler différents algorithmes d'ordonnancement de tâches temps réel.

## Algorithmes implémentés

- FCFS (First Come First Served)
- SJF (Shortest Job First)
- EDF (Earliest Deadline First)
- LLF (Least Laxity First)
- RM (Rate Monotonic)
- DM (Deadline Monotonic)

## Installation

1. Cloner le repository
```bash
git clone [URL_DU_REPO]
cd djjj
```

2. Créer un environnement virtuel
```bash
python -m venv env
source env/bin/activate  # Linux/Mac
env\Scripts\activate     # Windows
```

3. Installer les dépendances
```bash
pip install -r requirements.txt
```

4. Appliquer les migrations
```bash
python manage.py migrate
```

5. Lancer le serveur
```bash
python manage.py runserver
```

## Utilisation

1. Accéder à http://127.0.0.1:8000
2. Entrer les paramètres des tâches :
   - Temps d'arrivée
   - Durée
   - Deadline
   - Période
3. Choisir l'algorithme d'ordonnancement
4. Visualiser le diagramme de Gantt résultant

## Technologies utilisées

- Django 4.2
- Matplotlib
- Bootstrap 5
