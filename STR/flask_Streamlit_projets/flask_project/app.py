from flask import Flask, render_template, request, jsonify
import matplotlib.pyplot as plt
import io
import base64

app = Flask(__name__)

# Function to simulate FCFS
def fcfs(processes):
    processes.sort(key=lambda x: x['arrival'])
    time = 0
    gantt = []
    waiting_time = 0

    for process in processes:
        if time < process['arrival']:
            time = process['arrival']
        gantt.append({'start': time, 'name': process['name']})
        time += process['burst']
        gantt[-1]['end'] = time
        waiting_time += gantt[-1]['start'] - process['arrival']

    awt = waiting_time / len(processes)
    return gantt, awt

# Function to simulate SJF
def sjf(processes):
    processes.sort(key=lambda x: (x['arrival'], x['burst']))
    time = 0
    gantt = []
    waiting_time = 0
    ready_queue = []

    while processes or ready_queue:
        while processes and processes[0]['arrival'] <= time:
            ready_queue.append(processes.pop(0))

        if ready_queue:
            ready_queue.sort(key=lambda x: x['burst'])
            process = ready_queue.pop(0)
            gantt.append({'start': time, 'name': process['name']})
            time += process['burst']
            gantt[-1]['end'] = time
            waiting_time += gantt[-1]['start'] - process['arrival']
        else:
            time = processes[0]['arrival']

    awt = waiting_time / len(gantt)
    return gantt, awt

# Function to simulate LLF (Least Laxity First)
def llf(processes):
    time = 0
    gantt = []
    waiting_time = 0

    while processes:
        for process in processes:
            process['laxity'] = max(0, (process['deadline'] - (time + process['burst'])))
        processes.sort(key=lambda x: (x['laxity'], x['arrival']))
        if processes[0]['arrival'] <= time:
            process = processes.pop(0)
            gantt.append({'start': time, 'name': process['name']})
            time += process['burst']
            gantt[-1]['end'] = time
            waiting_time += gantt[-1]['start'] - process['arrival']
        else:
            time += 1

    awt = waiting_time / len(gantt)
    return gantt, awt

# Function to simulate EDF (Earliest Deadline First)
def edf(processes):
    processes.sort(key=lambda x: (x['arrival'], x['deadline']))
    time = 0
    gantt = []
    waiting_time = 0

    while processes:
        ready_queue = [p for p in processes if p['arrival'] <= time]
        if ready_queue:
            ready_queue.sort(key=lambda x: x['deadline'])
            process = ready_queue[0]
            processes.remove(process)
            gantt.append({'start': time, 'name': process['name']})
            time += process['burst']
            gantt[-1]['end'] = time
            waiting_time += gantt[-1]['start'] - process['arrival']
        else:
            time = processes[0]['arrival']

    awt = waiting_time / len(gantt)
    return gantt, awt

# Function to simulate RM (Rate Monotonic)
def rm(processes):
    processes.sort(key=lambda x: x['period'])
    time = 0
    gantt = []
    waiting_time = 0

    while processes:
        for process in processes:
            if process['arrival'] <= time:
                gantt.append({'start': time, 'name': process['name']})
                time += process['burst']
                gantt[-1]['end'] = time
                waiting_time += gantt[-1]['start'] - process['arrival']
                processes.remove(process)
                break
            else:
                time += 1

    awt = waiting_time / len(gantt)
    return gantt, awt

# Function to simulate DM (Deadline Monotonic)
def dm(processes):
    processes.sort(key=lambda x: x['deadline'])
    time = 0
    gantt = []
    waiting_time = 0

    while processes:
        for process in processes:
            if process['arrival'] <= time:
                gantt.append({'start': time, 'name': process['name']})
                time += process['burst']
                gantt[-1]['end'] = time
                waiting_time += gantt[-1]['start'] - process['arrival']
                processes.remove(process)
                break
            else:
                time += 1

    awt = waiting_time / len(gantt)
    return gantt, awt

# Function to draw Gantt chart
def draw_gantt(gantt):
    fig, ax = plt.subplots(figsize=(10, 6))  # Set a larger figure size
    y_labels = []
    for i, segment in enumerate(gantt):
        ax.broken_barh(
            [(segment['start'], segment['end'] - segment['start'])], 
            (i * 2, 1.5), 
            facecolors='tab:orange'
        )
        y_labels.append(segment['name'])
        ax.text(
            segment['start'] + (segment['end'] - segment['start']) / 2, 
            i * 2 + 0.75, 
            segment['name'], 
            ha='center', va='center', color='black'
        )
    
    ax.set_yticks([i * 2 + 0.75 for i in range(len(y_labels))])
    ax.set_yticklabels(y_labels)
    ax.set_xlabel('Time')
    ax.grid(True, axis='x', linestyle='--', alpha=0.7)
    ax.set_xlim(0, max(segment['end'] for segment in gantt) + 1)
    ax.set_ylim(0, len(gantt) * 2)
    
    buf = io.BytesIO()
    plt.savefig(buf, format='png', bbox_inches='tight')
    buf.seek(0)
    encoded = base64.b64encode(buf.read()).decode('utf-8')
    buf.close()
    return encoded

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/simulate', methods=['POST'])
def simulate():
    data = request.json
    algorithm = data['algorithm']
    processes = data['processes']

    if algorithm == 'fcfs':
        gantt, awt = fcfs(processes)
    elif algorithm == 'sjf':
        gantt, awt = sjf(processes)
    elif algorithm == 'llf':
        gantt, awt = llf(processes)
    elif algorithm == 'edf':
        gantt, awt = edf(processes)
    elif algorithm == 'rm':
        gantt, awt = rm(processes)
    elif algorithm == 'dm':
        gantt, awt = dm(processes)
    else:
        return jsonify({'error': 'Unsupported algorithm'})

    gantt_chart = draw_gantt(gantt)
    return jsonify({'gantt_chart': gantt_chart, 'awt': awt})

if __name__ == '__main__':
    app.run(debug=True)
