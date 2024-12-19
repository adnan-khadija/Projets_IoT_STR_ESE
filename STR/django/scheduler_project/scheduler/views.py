from django.shortcuts import render
import json

# Create your views here.

def dashboard(request):
    if request.method == 'POST':
        tasks = []
        for i in range(3):  # Pour 3 tâches
            task = {
                'id': i + 1,
                'arrival_time': int(request.POST.get(f'arrival_time_{i+1}')),
                'execution_time': int(request.POST.get(f'execution_time_{i+1}')),
                'deadline': int(request.POST.get(f'deadline_{i+1}'))
            }
            tasks.append(task)
        
        algorithm = request.POST.get('algorithm')
        
        context = {
            'tasks': tasks,
            'tasks_json': json.dumps(tasks),
            'algorithm': algorithm
        }
        return render(request, 'scheduler/dashboard.html', context)
    
    return render(request, 'scheduler/dashboard.html')
