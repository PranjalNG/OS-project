# FCFS CPU Scheduling Implementation Guide

## Introduction

This guide explains how to implement the First Come First Serve (FCFS) CPU scheduling algorithm with a web interface.

## Project Information

**University:** Graphic Era Hill University (GEHU), Bhimtal  
**Department:** School of Computing  
**Course:** Bachelor in Computer Application (BCA)  
**Semester:** 4th (2nd Year)  
**Subject:** Software Engineering  
**Project Group No.:** 44  
**Project Faculty:** Dr. Mukesh Joshi, Dr. Naveen Tewari

**Team Members:**
- Pranjal Nath Goswami (University Roll No: 2371289) - Project Lead & Backend Developer
- Diya Bisht (University Roll No: 2371101) - Frontend Developer
- Karan Singh (University Roll No: 2371171) - Algorithm Implementation
- Pradeep Singh Bora (University Roll No: 2371283) - Testing & Documentation

## Table of Contents
1. [Project Overview](#project-overview)
2. [What is FCFS?](#what-is-fcfs)
3. [Project Structure](#project-structure)
4. [Crow Framework Explained](#crow-framework-explained)
5. [FCFS Implementation](#fcfs-implementation)
6. [Web Interface Implementation](#web-interface-implementation)
7. [How to Run](#how-to-run)
8. [Common Issues](#common-issues)
9. [Practice Questions](#practice-questions)
10. [Additional Resources](#additional-resources)

## Project Overview
First Come First Serve (FCFS) CPU Scheduling Algorithm visualization project aapko operating system ke ek important concept ko practical way mein samajhne mein help karega. Is project mein hum:
1. FCFS algorithm ko C++ mein implement karenge
2. Web-based interface banayenge input lene aur results dikhane ke liye
3. Crow Framework ka use karenge web server banane ke liye

## What is FCFS?
FCFS (First Come First Serve) ek scheduling algorithm hai:
- Jo sabse pehle aaya, usko sabse pehle process kiya jata hai
- Non-preemptive: Jab tak process complete na ho, CPU release nahi hoga
- Simplest CPU scheduling algorithm

Example:
```
Process   Arrival Time   Burst Time
P1        0              5
P2        1              3
P3        2              8
```

Execution order: P1 → P2 → P3

## Project Structure
```
OS-project/
├── OS project (FCFS) using C++/
│   ├── program.cpp      # Main C++ implementation with Crow
│   ├── program.exe      # Compiled executable
│   └── index.html       # Frontend interface
├── README.md            # Project documentation
└── SYNOPSIS/            # Project synopsis files
```

## Crow Framework Explained
Crow ek C++ web framework hai jo web applications banane mein help karta hai.

### Why Use Crow?
- Lightweight hai - single header file
- Express.js (Node.js) jaisa syntax
- Built-in JSON support
- Routes easily define kar sakte hain
- Modern C++ (C++11 and above) features use karta hai

### Where We Use Crow in Our Code:
```cpp
// 1. Include Crow
#include "crow.h"

// 2. Create a Crow application
crow::SimpleApp app;

// 3. Define routes
CROW_ROUTE(app, "/")([](){
    // Code to handle root endpoint
});

// 4. Parse JSON from request
auto json = crow::json::load(req.body);

// 5. Return JSON response
return crow::response(result);
```

## FCFS Implementation
FCFS algorithm ke core components ye hain:

### 1. Process Structure
```cpp
struct Process {
    int pid;            // Process ID
    int arrival_time;   // When process arrives
    int burst_time;     // CPU time needed
    int completion_time;// When process finishes
    int turnaround_time;// Total time in system
    int waiting_time;   // Time spent waiting
};
```

### 2. Sorting Processes by Arrival Time
```cpp
// Sort processes by arrival time
void sort_by_arrival_time(Process proc[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(proc[j].arrival_time > proc[j+1].arrival_time) {
                // Swap processes
                Process temp = proc[j];
                proc[j] = proc[j+1];
                proc[j+1] = temp;
            }
        }
    }
}
```

### 3. Calculating Completion Time
```cpp
void calculate_completion_time(Process proc[], int n) {
    // First process
    proc[0].completion_time = proc[0].arrival_time + proc[0].burst_time;
    
    // For remaining processes
    for (int i = 1; i < n; i++) {
        // If process arrives after previous completes
        if (proc[i].arrival_time > proc[i-1].completion_time) {
            proc[i].completion_time = proc[i].arrival_time + proc[i].burst_time;
        } else {
            // If process arrives while previous is running
            proc[i].completion_time = proc[i-1].completion_time + proc[i].burst_time;
        }
    }
}
```

### 4. Calculating Turnaround Time
```cpp
void calculate_turnaround_time(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
    }
}
```

### 5. Calculating Waiting Time
```cpp
void calculate_waiting_time(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }
}
```

## Web Interface Implementation

### HTML Structure
Main HTML structure kuch aise hoga:
```html
<div class="container">
    <h1>FCFS CPU Scheduling Calculator</h1>
    
    <div class="input-section">
        <!-- Process Input Form -->
        <form id="process-form">
            <input type="number" id="pid" placeholder="Process ID">
            <input type="number" id="arrival" placeholder="Arrival Time">
            <input type="number" id="burst" placeholder="Burst Time">
            <button type="submit">Add Process</button>
        </form>
    </div>
    
    <div class="process-list">
        <!-- Process Table -->
        <table id="process-table">
            <thead>
                <tr>
                    <th>PID</th>
                    <th>Arrival Time</th>
                    <th>Burst Time</th>
                </tr>
            </thead>
            <tbody>
                <!-- Processes will be added here by JavaScript -->
            </tbody>
        </table>
        <button id="calculate-btn">Calculate</button>
    </div>
    
    <div class="results-section">
        <!-- Results will be displayed here -->
    </div>
</div>
```

### JavaScript Logic
```javascript
// Add a new process
function addProcess() {
    const pid = document.getElementById('pid').value;
    const arrival = document.getElementById('arrival').value;
    const burst = document.getElementById('burst').value;
    
    // Add to table
    const row = `<tr>
        <td>${pid}</td>
        <td>${arrival}</td>
        <td>${burst}</td>
    </tr>`;
    
    document.querySelector('#process-table tbody').innerHTML += row;
    
    // Store in processes array
    processes.push({
        pid: parseInt(pid),
        arrival: parseInt(arrival),
        burst: parseInt(burst)
    });
}

// Calculate FCFS
function calculateFCFS() {
    fetch('/fcfs', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ processes: processes })
    })
    .then(response => response.json())
    .then(data => displayResults(data))
    .catch(error => console.error('Error:', error));
}
```

## How to Run

### Prerequisites
1. C++ compiler (G++ ya Visual C++)
2. Web browser

### Steps
1. Source code clone karein:
   ```
   git clone https://github.com/PranjalNG/OS-project.git
   cd OS-project
   ```

2. C++ program ko compile karein:
   ```
   g++ program.cpp -o program.exe -std=c++17
   ```

3. Program run karein:
   ```
   ./program.exe
   ```

4. Web browser mein visit karein:
   ```
   http://localhost:5000
   ```

## Common Issues

### Compilation Errors
1. **Missing Crow header**: Make sure you include Crow header correctly
   ```cpp
   #include "crow.h"
   ```

2. **C++ version error**: Modern C++ version (C++11 or higher) needed
   ```
   g++ program.cpp -o program.exe -std=c++17
   ```

### Runtime Errors
1. **Port already in use**: If port 5000 is already in use, change port in code
   ```cpp
   app.port(5000).multithreaded().run();
   ```

2. **CORS errors**: If testing with separate frontend
   ```cpp
   // Add CORS headers
   app.after_request([](crow::response& res) {
       res.add_header("Access-Control-Allow-Origin", "*");
       res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
       res.add_header("Access-Control-Allow-Headers", "Content-Type");
   });
   ```

## Practice Questions
1. FCFS algorithm mein average waiting time kaise calculate karte hain?
2. FCFS ke advantages aur disadvantages kya hain?
3. Convoy effect kya hai aur yeh FCFS mein kaise hota hai?
4. Crow framework mein JSON data kaise parse karte hain?
5. C++ mein struct aur class mein kya difference hai?

## Additional Resources

### Books
1. Operating System Concepts by Silberschatz, Galvin, and Gagne
2. Modern Operating Systems by Andrew S. Tanenbaum

### Online Resources
1. GeeksforGeeks - FCFS Scheduling
2. Crow framework documentation
3. C++ Reference

### Video Tutorials
1. Operating Systems - CPU Scheduling Algorithms
2. Building Web Servers with C++ 