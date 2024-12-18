import streamlit as st
import matplotlib.pyplot as plt

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

# Function to simulate LLF
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

# Function to simulate EDF
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

# Function to simulate RM
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

# Function to simulate DM
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
    fig, ax = plt.subplots(figsize=(10, 6))
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

    return fig

# Streamlit app
st.title("Scheduling Algorithm Simulator")
st.sidebar.header("Simulation Parameters")

# Select algorithm
algorithm = st.sidebar.selectbox(
    "Choose Scheduling Algorithm:",
    ["FCFS", "SJF", "LLF", "EDF", "RM", "DM"]
)

# Input number of processes
num_processes = st.sidebar.number_input("Number of Processes:", min_value=1, value=3)

# Process input form
st.sidebar.subheader("Processes")
processes = []
for i in range(num_processes):
    st.sidebar.write(f"Process {i+1}")
    name = f"P{i+1}"
    arrival = st.sidebar.number_input(f"Arrival Time of {name}:", min_value=0, value=i, key=f"arrival_{i}")
    burst = st.sidebar.number_input(f"Burst Time of {name}:", min_value=1, value=1, key=f"burst_{i}")
    deadline = st.sidebar.number_input(f"Deadline of {name}:", min_value=1, value=10, key=f"deadline_{i}")
    period = st.sidebar.number_input(f"Period of {name}:", min_value=1, value=15, key=f"period_{i}")
    processes.append({'name': name, 'arrival': arrival, 'burst': burst, 'deadline': deadline, 'period': period})

# Simulate button
if st.sidebar.button("Simulate"):
    if algorithm == "FCFS":
        gantt, awt = fcfs(processes)
    elif algorithm == "SJF":
        gantt, awt = sjf(processes)
    elif algorithm == "LLF":
        gantt, awt = llf(processes)
    elif algorithm == "EDF":
        gantt, awt = edf(processes)
    elif algorithm == "RM":
        gantt, awt = rm(processes)
    elif algorithm == "DM":
        gantt, awt = dm(processes)
    else:
        st.error("Unsupported Algorithm")
        gantt, awt = [], 0

    # Display results
    st.subheader("Simulation Results")
    st.write(f"**Average Waiting Time:** {awt:.2f}")

    # Gantt chart
    st.subheader("Gantt Chart")
    fig = draw_gantt(gantt)
    st.pyplot(fig)
