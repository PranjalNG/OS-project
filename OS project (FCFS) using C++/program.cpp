/**
 * First Come First Serve (FCFS) CPU Scheduling Algorithm Implementation
 * Using Crow Framework for Web Interface
 * This is a beginner-friendly implementation with detailed comments
 */

#include "crow.h"  // Include Crow framework
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

// Maximum number of processes we can handle
const int MAX_PROCESSES = 10;

/**
 * Process structure to store information about each process
 * pid: Process ID
 * arrival_time: When the process arrives in the ready queue
 * burst_time: How long the process needs to run
 * completion_time: When the process finishes execution
 * turnaround_time: Total time from arrival to completion
 * waiting_time: Time spent waiting in the ready queue
 */
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

/**
 * Calculates completion time for each process using FCFS algorithm
 * Processes are first sorted by arrival time
 * First process starts at its arrival time
 * Subsequent processes start after previous process completes
 */
void findCompletionTime(Process proc[], int n) {
    // Sort processes by arrival time (key requirement for FCFS)
    // Simple bubble sort for beginners
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
    
    // First process starts at its arrival time
    proc[0].completion_time = proc[0].arrival_time + proc[0].burst_time;
    
    // Calculate completion time for remaining processes
    for (int i = 1; i < n; i++) {
        // If process arrives after previous process completes
        if (proc[i].arrival_time > proc[i-1].completion_time) {
            proc[i].completion_time = proc[i].arrival_time + proc[i].burst_time;
        } else {
            // If process arrives while previous process is running
            proc[i].completion_time = proc[i-1].completion_time + proc[i].burst_time;
        }
    }
}

/**
 * Calculates turnaround time for each process
 * Turnaround time = Completion time - Arrival time
 */
void findTurnaroundTime(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
    }
}

/**
 * Calculates waiting time for each process
 * Waiting time = Turnaround time - Burst time
 */
void findWaitingTime(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }
}

/**
 * Main FCFS calculation function that calls all helper functions
 */
void findFCFS(Process proc[], int n) {
    findCompletionTime(proc, n);
    findTurnaroundTime(proc, n);
    findWaitingTime(proc, n);
}

int main() {
    // Create a new Crow application
    crow::SimpleApp app;

    // Define a route for the root path ("/")
    CROW_ROUTE(app, "/")([](){
        // Read and return the HTML file
        ifstream file("index.html");
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    });

    // Define a route for FCFS calculation ("/fcfs")
    CROW_ROUTE(app, "/fcfs").methods("POST"_method)
    ([](const crow::request& req){
        try {
            // Parse JSON from request body
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400, "Invalid JSON");
            }

            // Create array to store processes
            Process processes[MAX_PROCESSES];
            int processCount = 0;

            // Parse each process from JSON
            for (const auto& process : json["processes"]) {
                if (processCount >= MAX_PROCESSES) {
                    return crow::response(400, "Too many processes. Maximum allowed: " + to_string(MAX_PROCESSES));
                }

                processes[processCount].pid = process["pid"].i();
                processes[processCount].arrival_time = process["arrival"].i();
                processes[processCount].burst_time = process["burst"].i();
                
                // Only add valid processes
                if (processes[processCount].burst_time > 0) {
                    processCount++;
                }
            }

            // If no valid processes found
            if (processCount == 0) {
                return crow::response(400, "No valid processes provided");
            }

            // Calculate FCFS
            findFCFS(processes, processCount);

            // Create response JSON
            crow::json::wvalue result;
            for (int i = 0; i < processCount; i++) {
                result[i]["pid"] = processes[i].pid;
                result[i]["arrival_time"] = processes[i].arrival_time;
                result[i]["burst_time"] = processes[i].burst_time;
                result[i]["completion_time"] = processes[i].completion_time;
                result[i]["turnaround_time"] = processes[i].turnaround_time;
                result[i]["waiting_time"] = processes[i].waiting_time;
            }

            // Return success response
            return crow::response(result);
        }
        catch (const exception& e) {
            return crow::response(500, string("Error: ") + e.what());
        }
    });

    // Start the server on port 5000
    app.port(5000).multithreaded().run();

    return 0;
}
