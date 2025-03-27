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
5. [Backend Code Explanation](#backend-code-explanation)
6. [Frontend Code Explanation](#frontend-code-explanation)
7. [How to Run the Project](#how-to-run-the-project)
8. [Common Questions](#common-questions)

## Project Overview
Ye project ek CPU scheduling algorithm implement karta hai jisme processes ko unke arrival time ke basis par process kiya jata hai. Project mein ek web interface hai jisse aap processes add kar sakte hain aur results dekh sakte hain.

## What is FCFS?
FCFS (First Come First Serve) ek simple CPU scheduling algorithm hai:
- Jo process pehle aata hai, usko pehle process kiya jata hai
- Jaise bank queue mein jo pehle aata hai, uski pehle service hoti hai
- Har process ka arrival time aur burst time (execution time) hota hai

Example:
```
Process 1: Arrival = 0, Burst = 5
Process 2: Arrival = 2, Burst = 3
Process 3: Arrival = 4, Burst = 2

Execution Order:
1. Process 1 (0-5)
2. Process 2 (5-8)
3. Process 3 (8-10)
```

## Project Structure
```
OS project (FCFS) using C++/
├── program.cpp      # Main server code
├── program.exe     # Compiled program
├── index.html      # Web interface
└── crow/           # Crow framework files
```

## Crow Framework Explained
Crow ek C++ web framework hai jo web applications banane mein help karta hai.

### Why Use Crow?
1. **Easy to Use**: 
   - Web server create karna aasan hai
   - JSON handling simple hai
   - Routes define karna easy hai

2. **Features**:
   - HTTP server
   - JSON parsing
   - Routing
   - Error handling

### Where We Use Crow in Our Code:
```cpp
// 1. Include Crow
#include "crow.h"

// 2. Create Web Application
crow::SimpleApp app;

// 3. Define Routes
CROW_ROUTE(app, "/")([](){
    // Handle home page
});

// 4. Handle JSON Data
auto json = crow::json::load(req.body);

// 5. Send Response
return crow::response(result);

// 6. Start Server
app.port(5000).multithreaded().run();
```

## Backend Code Explanation

### 1. Process Structure
```cpp
struct Process {
    int pid;              // Process ID
    int arrival_time;     // When process arrives
    int burst_time;       // How long it needs to run
    int completion_time;  // When it finishes
    int turnaround_time;  // Total time taken
    int waiting_time;     // Time spent waiting
};
```

### 2. Main Functions
```cpp
// Sort processes by arrival time
void findCompletionTime(Process proc[], int n) {
    // Bubble sort for beginners
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
    
    // Calculate completion times
    proc[0].completion_time = proc[0].arrival_time + proc[0].burst_time;
    for (int i = 1; i < n; i++) {
        if (proc[i].arrival_time > proc[i-1].completion_time) {
            proc[i].completion_time = proc[i].arrival_time + proc[i].burst_time;
        } else {
            proc[i].completion_time = proc[i-1].completion_time + proc[i].burst_time;
        }
    }
}

// Calculate turnaround time
void findTurnaroundTime(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
    }
}

// Calculate waiting time
void findWaitingTime(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }
}
```

## Frontend Code Explanation

### 1. HTML Structure
```html
<!DOCTYPE html>
<html>
<head>
    <title>FCFS Scheduler</title>
    <style>
        /* CSS for styling */
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
        }
        .form-group {
            margin-bottom: 15px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
        }
        th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: left;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>FCFS CPU Scheduler</h1>
        
        <!-- Process Input Form -->
        <form id="processForm">
            <div class="form-group">
                <label>Process ID:</label>
                <input type="number" id="pid" required>
            </div>
            <div class="form-group">
                <label>Arrival Time:</label>
                <input type="number" id="arrival" required>
            </div>
            <div class="form-group">
                <label>Burst Time:</label>
                <input type="number" id="burst" required>
            </div>
            <button type="submit">Add Process</button>
        </form>

        <!-- Process List -->
        <h2>Processes</h2>
        <table id="processList">
            <thead>
                <tr>
                    <th>PID</th>
                    <th>Arrival Time</th>
                    <th>Burst Time</th>
                    <th>Action</th>
                </tr>
            </thead>
            <tbody></tbody>
        </table>

        <!-- Calculate Button -->
        <button onclick="calculateFCFS()">Calculate FCFS</button>

        <!-- Results Table -->
        <h2>Results</h2>
        <table id="results">
            <thead>
                <tr>
                    <th>PID</th>
                    <th>Arrival Time</th>
                    <th>Burst Time</th>
                    <th>Completion Time</th>
                    <th>Turnaround Time</th>
                    <th>Waiting Time</th>
                </tr>
            </thead>
            <tbody></tbody>
        </table>
    </div>

    <script>
        // JavaScript code here
    </script>
</body>
</html>
```

### 2. JavaScript Code
```javascript
// Store processes
let processes = [];

// Add process to list
function addProcess() {
    const pid = document.getElementById('pid').value;
    const arrival = document.getElementById('arrival').value;
    const burst = document.getElementById('burst').value;

    processes.push({
        pid: parseInt(pid),
        arrival: parseInt(arrival),
        burst: parseInt(burst)
    });

    updateProcessList();
    document.getElementById('processForm').reset();
}

// Update process list table
function updateProcessList() {
    const tbody = document.querySelector('#processList tbody');
    tbody.innerHTML = '';

    processes.forEach((process, index) => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${process.pid}</td>
            <td>${process.arrival}</td>
            <td>${process.burst}</td>
            <td><button onclick="removeProcess(${index})">Remove</button></td>
        `;
        tbody.appendChild(row);
    });
}

// Remove process
function removeProcess(index) {
    processes.splice(index, 1);
    updateProcessList();
}

// Calculate FCFS
async function calculateFCFS() {
    try {
        const response = await fetch('http://localhost:5000/fcfs', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ processes: processes })
        });

        const results = await response.json();
        displayResults(results);
    } catch (error) {
        console.error('Error:', error);
        alert('Error calculating FCFS');
    }
}

// Display results
function displayResults(results) {
    const tbody = document.querySelector('#results tbody');
    tbody.innerHTML = '';

    results.forEach(result => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${result.pid}</td>
            <td>${result.arrival_time}</td>
            <td>${result.burst_time}</td>
            <td>${result.completion_time}</td>
            <td>${result.turnaround_time}</td>
            <td>${result.waiting_time}</td>
        `;
        tbody.appendChild(row);
    });
}
```

## How to Run the Project

### Prerequisites:
1. C++ compiler (g++)
2. Crow framework
3. Web browser

### Steps:
1. **Compile the C++ program**:
   ```bash
   g++ program.cpp -o program.exe -I./crow/include -std=c++17
   ```

2. **Run the server**:
   ```bash
   ./program.exe
   ```

3. **Open the web interface**:
   - Open `index.html` in your browser
   - Add processes using the form
   - Click "Calculate FCFS" to see results

## Common Questions

### 1. What is Crow Framework?
Crow ek C++ web framework hai jo web applications banane mein help karta hai. Isme web server, JSON handling, aur routing jaise features hote hain.

### 2. Why Use Crow?
- Web server create karna aasan hai
- JSON data handle karna simple hai
- Routes define karna easy hai
- Error handling built-in hai

### 3. How Does FCFS Work?
- Processes ko arrival time ke basis par sort karta hai
- Pehle aane wale process ko pehle execute karta hai
- Har process ka completion time, turnaround time, aur waiting time calculate karta hai

### 4. How to Add New Features?
1. Frontend mein form fields add karein
2. Backend mein new functions add karein
3. API endpoints update karein
4. JavaScript mein new functionality add karein

### 5. How to Debug?
1. Browser console check karein (F12)
2. Server logs check karein
3. Print statements add karein
4. Error handling improve karein 