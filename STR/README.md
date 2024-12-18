# Simulateur d'Algorithmes d'Ordonnancement

Ce projet est une application web interactive permettant de simuler plusieurs algorithmes d'ordonnancement, notamment **FCFS**, **SJF**, **EDF**, **LLF**, **RM**, et **DM**. Il utilise trois technologies principales pour la démonstration : **Streamlit**, **Flask**, et **Django**.

---

## Fonctionnalités principales

1. **Sélection des algorithmes** :
   - **First Come, First Served (FCFS)**
   - **Shortest Job First (SJF)**
   - **Earliest Deadline First (EDF)**
   - **Least Laxity First (LLF)**
   - **Rate Monotonic (RM)**
   - **Deadline Monotonic (DM)**

2. **Entrée dynamique** :
   - Ajout de plusieurs processus avec des paramètres personnalisés :
     - Temps d'arrivée
     - Temps d'exécution (*Burst Time*)
     - Deadline et période pour les algorithmes temps réel

3. **Affichage des résultats** :
   - **Temps d'attente moyen** calculé pour chaque algorithme
   - **Diagramme de Gantt** généré dynamiquement avec Matplotlib

---

## Technologies utilisées

- **Streamlit** : Interface utilisateur interactive et facile à utiliser.
- **Flask** : Backend léger pour gérer la logique des algorithmes.
- **Django** : Framework complet pour la gestion du projet et son intégration.

---

## Installation et Exécution

### Prérequis

- **Python 3.x**
- Installer les dépendances nécessaires avec la commande suivante :
  ```bash
  pip install streamlit flask django matplotlib
  ```

### Étapes

1. **Cloner le dépôt** :
   ```bash
   git clone https://github.com/nom-utilisateur/nom-du-repo.git
   cd nom-du-repo
   ```

2. **Démarrer Flask** :
   - Depuis le dossier *Flask* :
     ```bash
     python app.py
     ```

3. **Démarrer Django** :
   - Depuis le dossier *Django* :
     ```bash
     python manage.py runserver
     ```

4. **Démarrer Streamlit** :
   - Depuis le dossier *Streamlit* :
     ```bash
     streamlit run app.py
     ```

---

## Diagramme de Gantt et Résultats

- **Diagramme de Gantt** : Visualisation claire de l'exécution des processus.
- **Temps moyen d'attente** : Affiché directement après chaque simulation.

---
## Captures d'écran
![image](https://github.com/user-attachments/assets/c445a524-1aa3-478c-8a2a-a1bc1f1e13b3)
![image](https://github.com/user-attachments/assets/5d915cd8-649f-49a4-8231-cec8db656696)

