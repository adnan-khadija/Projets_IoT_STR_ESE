from django.shortcuts import render
import matplotlib.pyplot as plt
import io
import base64
import numpy as np
import math

def calculate_hyperperiod(periods):
    """Calcule l'hyperpériode (PPCM des périodes)"""
    def lcm(a, b):
        return abs(a * b) // math.gcd(a, b)
    
    result = periods[0]
    for i in range(1, len(periods)):
        result = lcm(result, periods[i])
    return result

def draw_gantt(schedule, tasks):
    """Dessine le diagramme de Gantt"""
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

def schedule_tasks(tasks, algorithm):
    """Planifie les tâches selon l'algorithme choisi"""
    # Calculer l'hyperpériode pour les algorithmes périodiques
    hyperperiod = calculate_hyperperiod([task['period'] for task in tasks])
    
    current_time = 0
    schedule = []
    ready_tasks = []
    
    while current_time < hyperperiod:
        # Ajouter les nouvelles instances de tâches
        for task in tasks:
            if current_time % task['period'] == 0:
                new_task = task.copy()
                new_task['remaining'] = task['duration']
                new_task['absolute_deadline'] = current_time + task['deadline']
                ready_tasks.append(new_task)
        
        if ready_tasks:
            # Sélectionner la prochaine tâche selon l'algorithme
            if algorithm == 'fcfs':
                next_task = min(ready_tasks, key=lambda t: t['arrival'])
            elif algorithm == 'sjf':
                next_task = min(ready_tasks, key=lambda t: t['duration'])
            elif algorithm == 'edf':
                next_task = min(ready_tasks, key=lambda t: t['absolute_deadline'])
            elif algorithm == 'llf':
                # Calculer la laxité pour chaque tâche
                for task in ready_tasks:
                    task['laxity'] = task['absolute_deadline'] - current_time - task['remaining']
                next_task = min(ready_tasks, key=lambda t: t['laxity'])
            elif algorithm == 'rm':
                next_task = min(ready_tasks, key=lambda t: t['period'])
            elif algorithm == 'dm':
                next_task = min(ready_tasks, key=lambda t: t['deadline'])
            else:
                raise ValueError(f"Algorithme non supporté : {algorithm}")
            
            # Exécuter la tâche pour une unité de temps
            schedule.append({
                'task': next_task['name'],
                'start': current_time,
                'end': current_time + 1
            })
            
            next_task['remaining'] -= 1
            
            # Retirer la tâche si elle est terminée
            if next_task['remaining'] == 0:
                ready_tasks.remove(next_task)
        
        current_time += 1
        
        # Retirer les tâches qui ont dépassé leur deadline
        ready_tasks = [t for t in ready_tasks if t['absolute_deadline'] >= current_time]
    
    # Fusionner les intervalles consécutifs pour le même processus
    merged_schedule = []
    current = None
    
    for entry in schedule:
        if current is None:
            current = entry.copy()
        elif current['task'] == entry['task'] and current['end'] == entry['start']:
            current['end'] = entry['end']
        else:
            merged_schedule.append(current)
            current = entry.copy()
    
    if current:
        merged_schedule.append(current)
    
    return merged_schedule

def dashboard(request):
    context = {}
    
    if request.method == 'POST':
        # Récupérer les données du formulaire
        algorithm = request.POST.get('algorithm', 'fcfs')
        
        # Créer la liste des tâches
        tasks = []
        for i in range(1, 4):
            task = {
                'name': f'T{i}',
                'arrival': int(request.POST.get(f'arrival_{i}')),
                'duration': int(request.POST.get(f'duration_{i}')),
                'deadline': int(request.POST.get(f'deadline_{i}')),
                'period': int(request.POST.get(f'period_{i}'))
            }
            tasks.append(task)
        
        # Planifier les tâches
        schedule = schedule_tasks(tasks, algorithm)
        
        # Calculer le temps d'attente moyen
        total_waiting_time = 0
        task_start_times = {}
        
        for entry in schedule:
            task_name = entry['task']
            if task_name not in task_start_times:
                task = next(t for t in tasks if t['name'] == task_name)
                waiting_time = entry['start'] - task['arrival']
                total_waiting_time += waiting_time
                task_start_times[task_name] = True
        
        awt = total_waiting_time / len(tasks)
        
        # Générer le diagramme de Gantt
        gantt_chart = draw_gantt(schedule, tasks)
        
        context.update({
            'gantt_chart': gantt_chart,
            'awt': awt,
            'schedule': schedule
        })
    
    return render(request, 'edf_scheduler/dashboard.html', context)
