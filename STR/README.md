
# Simulateur d'Algorithmes d'Ordonnancement

Ce projet est une application web interactive qui permet de simuler plusieurs algorithmes d'ordonnancement, notamment *FCFS, **SJF, **EDF, **LLF, **RM, et **DM. Le projet utilise trois technologies pour la démonstration : **Streamlit, **Flask, et **Django*.

---

## Fonctionnalités principales

1. *Sélection des algorithmes* :
   - First Come, First Served (FCFS)
   - Shortest Job First (SJF)
   - Earliest Deadline First (EDF)
   - Least Laxity First (LLF)
   - Rate Monotonic (RM)
   - Deadline Monotonic (DM)

2. *Entrée dynamique* :
   - Ajout de plusieurs processus avec des paramètres personnalisés :
     - Temps d'arrivée
     - Temps d'exécution (Burst Time)
     - Deadline et période pour les algorithmes temps réel

3. *Affichage des résultats* :
   - *Temps d'attente moyen* pour les algorithmes
   - *Diagramme de Gantt* généré dynamiquement avec Matplotlib

---

## Technologies utilisées

- *Streamlit* : Interface utilisateur interactive et simple.
- *Flask* : Backend léger pour gérer les algorithmes et la logique.
- *Django* : Framework complet pour la gestion du projet et son intégration.

---

## Installation et Exécution

### Prérequis

- Python 3.x
- Installations requises :
   bash
   pip install streamlit flask django matplotlib
   

### Étapes

1. *Clone le dépôt* :
   bash
   git clone <URL_DU_DEPOT>
   cd <nom_du_projet>
   

2. *Démarrer Flask* :
   - Depuis le dossier *Flask* :
     bash
     python app.py
     

3. *Démarrer Django* :
   - Depuis le dossier *Django* :
     bash
     python manage.py runserver
     

4. *Démarrer Streamlit* :
   - Depuis le dossier *Streamlit* :
     bash
     streamlit run app.py
     

---

## Diagramme de Gantt et Résultats

- *Gantt Chart* : Visualisation claire de l'exécution des processus.
- *Temps moyen d'attente* : Affiché directement après la simulation.

---

## Captures d'écran
![image](https://github.com/user-attachments/assets/c445a524-1aa3-478c-8a2a-a1bc1f1e13b3)
![image](https://github.com/user-attachments/assets/5d915cd8-649f-49a4-8231-cec8db656696)

