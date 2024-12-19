from django.shortcuts import render
from django.http import JsonResponse
import json
import matplotlib.pyplot as plt
import io
import base64
import numpy as np

class Task:
    def __init__(self, name, arrival, duration, deadline):
        self.name = name
        self.arrival = arrival
        self.duration = duration
        self.deadline = deadline

def schedule_tasks(tasks, algorithm):
    """
    Étape 2 : Calculer le Temps de Processeur (TP)
    Retourne un tableau de début-fin pour chaque tâche
    """
    current_time = 0
    schedule = []
    remaining_tasks = tasks.copy()
    
    while remaining_tasks:
        # Filtrer les tâches disponibles au temps actuel
        available_tasks = [t for t in remaining_tasks if t.arrival <= current_time]
        
        if not available_tasks:
            current_time = min(t.arrival for t in remaining_tasks)
            continue
            
        # Sélectionner la prochaine tâche selon l'algorithme
        if algorithm == 'fcfs':
            next_task = min(available_tasks, key=lambda t: t.arrival)
        elif algorithm == 'sjf':
            next_task = min(available_tasks, key=lambda t: t.duration)
        elif algorithm == 'edf':
            next_task = min(available_tasks, key=lambda t: t.arrival + t.deadline)
        else:
            raise ValueError(f"Algorithme non supporté : {algorithm}")
            
        # Ajouter la tâche au planning
        schedule.append({
            'task': next_task.name,
            'start': current_time,
            'end': current_time + next_task.duration
        })
        
        current_time += next_task.duration
        remaining_tasks.remove(next_task)
    
    return schedule

def draw_gantt(schedule, tasks):
    """
    Étape 3 : Dessiner le diagramme de Gantt
    """
    fig, ax = plt.subplots(figsize=(12, 6))
    
    # Créer une liste unique de tâches dans l'ordre d'apparition
    task_names = []
    for entry in schedule:
        if entry['task'] not in task_names:
            task_names.append(entry['task'])
    
    # Définir les couleurs
    colors = plt.cm.Set3(np.linspace(0, 1, len(task_names)))
    color_map = dict(zip(task_names, colors))
    
    # Tracer les barres du Gantt
    for i, entry in enumerate(schedule):
        ax.barh(y=task_names.index(entry['task']),
                width=entry['end'] - entry['start'],
                left=entry['start'],
                color=color_map[entry['task']],
                edgecolor='black')
        
        # Ajouter les étiquettes de durée
        duration = entry['end'] - entry['start']
        ax.text(entry['start'] + duration/2,
                task_names.index(entry['task']),
                f"{duration}",
                ha='center', va='center')
    
    # Configurer les axes
    ax.set_yticks(range(len(task_names)))
    ax.set_yticklabels(task_names)
    ax.set_xlabel('Temps')
    ax.set_title('Diagramme de Gantt')
    ax.grid(True, axis='x')
    
    # Sauvegarder l'image
    buffer = io.BytesIO()
    plt.savefig(buffer, format='png', bbox_inches='tight')
    buffer.seek(0)
    image_png = buffer.getvalue()
    buffer.close()
    plt.close()
    
    return base64.b64encode(image_png).decode()

def simulate(request):
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            algorithm = data.get('algorithm', 'fcfs').lower()
            processes_data = data.get('processes', [])
            
            # Étape 1 : Définir les tâches
            tasks = [
                Task(
                    name=p['name'],
                    arrival=p['arrival'],
                    duration=p['burst'],
                    deadline=p['deadline']
                ) for p in processes_data
            ]
            
            # Étape 2 : Calculer le planning
            schedule = schedule_tasks(tasks, algorithm)
            
            # Étape 3 : Générer le diagramme de Gantt
            gantt_chart = draw_gantt(schedule, tasks)
            
            # Calculer le temps d'attente moyen
            total_waiting_time = 0
            for task in tasks:
                task_schedule = next(s for s in schedule if s['task'] == task.name)
                waiting_time = task_schedule['start'] - task.arrival
                total_waiting_time += waiting_time
            
            awt = total_waiting_time / len(tasks)
            
            return JsonResponse({
                'gantt_chart': gantt_chart,
                'awt': awt,
                'schedule': schedule
            })
            
        except Exception as e:
            return JsonResponse({'error': str(e)})
    
    return render(request, 'scheduler/index.html')
