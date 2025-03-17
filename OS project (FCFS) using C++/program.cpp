#include <iostream>
#include <vector>
#include <algorithm> // For sorting
#include <string>
#include <sstream>
#include <fstream>

// Include Windows-specific networking headers
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

void findCompletionTime(std::vector<Process>& proc) {
    // Sort processes by arrival time (key requirement for FCFS)
    std::sort(proc.begin(), proc.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });
    
    // First process completion time
    proc[0].completion_time = proc[0].arrival_time + proc[0].burst_time;
    
    // For remaining processes
    for (size_t i = 1; i < proc.size(); i++) {
        // If the process arrives after previous process completes
        if (proc[i].arrival_time > proc[i-1].completion_time) {
            proc[i].completion_time = proc[i].arrival_time + proc[i].burst_time;
        } else {
            // If the process arrives while previous process is still running
            proc[i].completion_time = proc[i-1].completion_time + proc[i].burst_time;
        }
    }
}

void findTurnaroundTime(std::vector<Process>& proc) {
    for (size_t i = 0; i < proc.size(); i++) {
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
    }
}

void findWaitingTime(std::vector<Process>& proc) {
    for (size_t i = 0; i < proc.size(); i++) {
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }
}

void findFCFS(std::vector<Process>& proc) {
    findCompletionTime(proc);
    findTurnaroundTime(proc);
    findWaitingTime(proc);
}

// Simple JSON parsing function
std::vector<Process> parseJSON(const std::string& json) {
    std::vector<Process> processes;
    
    // Very basic JSON parsing - in real code, use a proper JSON library
    size_t processesPos = json.find("\"processes\"");
    if (processesPos == std::string::npos) return processes;
    
    size_t arrStart = json.find('[', processesPos);
    size_t arrEnd = json.find(']', arrStart);
    
    if (arrStart == std::string::npos || arrEnd == std::string::npos) return processes;
    
    std::string processesArray = json.substr(arrStart + 1, arrEnd - arrStart - 1);
    
    size_t pos = 0;
    while (pos < processesArray.length()) {
        size_t objStart = processesArray.find('{', pos);
        if (objStart == std::string::npos) break;
        
        size_t objEnd = processesArray.find('}', objStart);
        if (objEnd == std::string::npos) break;
        
        std::string processObj = processesArray.substr(objStart, objEnd - objStart + 1);
        
        // Extract values
        Process p;
        
        // Find pid
        size_t pidPos = processObj.find("\"pid\"");
        if (pidPos != std::string::npos) {
            size_t colonPos = processObj.find(':', pidPos);
            size_t commaPos = processObj.find(',', colonPos);
            if (commaPos == std::string::npos) commaPos = processObj.find('}', colonPos);
            
            std::string pidStr = processObj.substr(colonPos + 1, commaPos - colonPos - 1);
            p.pid = std::stoi(pidStr);
        }
        
        // Find arrival time
        size_t arrivalPos = processObj.find("\"arrival\"");
        if (arrivalPos != std::string::npos) {
            size_t colonPos = processObj.find(':', arrivalPos);
            size_t commaPos = processObj.find(',', colonPos);
            if (commaPos == std::string::npos) commaPos = processObj.find('}', colonPos);
            
            std::string arrivalStr = processObj.substr(colonPos + 1, commaPos - colonPos - 1);
            p.arrival_time = std::stoi(arrivalStr);
        }
        
        // Find burst time
        size_t burstPos = processObj.find("\"burst\"");
        if (burstPos != std::string::npos) {
            size_t colonPos = processObj.find(':', burstPos);
            size_t commaPos = processObj.find(',', colonPos);
            if (commaPos == std::string::npos) commaPos = processObj.find('}', colonPos);
            
            std::string burstStr = processObj.substr(colonPos + 1, commaPos - colonPos - 1);
            p.burst_time = std::stoi(burstStr);
        }
        
        // Only add if all values are set and burst time is positive
        if (p.burst_time > 0) {
            processes.push_back(p);
        }
        
        pos = objEnd + 1;
    }
    
    return processes;
}

// Generate JSON response
std::string generateJSON(const std::vector<Process>& processes) {
    std::stringstream ss;
    
    ss << "[";
    
    for (size_t i = 0; i < processes.size(); i++) {
        if (i > 0) ss << ",";
        
        ss << "{";
        ss << "\"pid\":" << processes[i].pid << ",";
        ss << "\"arrival_time\":" << processes[i].arrival_time << ",";
        ss << "\"burst_time\":" << processes[i].burst_time << ",";
        ss << "\"completion_time\":" << processes[i].completion_time << ",";
        ss << "\"turnaround_time\":" << processes[i].turnaround_time << ",";
        ss << "\"waiting_time\":" << processes[i].waiting_time;
        ss << "}";
    }
    
    ss << "]";
    
    return ss.str();
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Setup the server address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;          // IPv4
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // Any available address
    serverAddress.sin_port = htons(5000);        // Port 5000

    // Bind the socket
    if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server started on port 5000..." << std::endl;
    std::cout << "Open index.html in your browser to use the application" << std::endl;

    // Main server loop
    while (true) {
        // Accept a client socket
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        // Receive data from client
        char recvBuffer[4096];
        int bytesReceived = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
        
        if (bytesReceived > 0) {
            // Null-terminate the received data
            recvBuffer[bytesReceived] = '\0';
            
            // Parse the HTTP request
            std::string request(recvBuffer);
            
            // Check if it's a POST request to /fcfs
            if (request.find("POST /fcfs HTTP/1.1") != std::string::npos) {
                // Find the request body (JSON data)
                size_t bodyStart = request.find("\r\n\r\n");
                if (bodyStart != std::string::npos) {
                    std::string jsonData = request.substr(bodyStart + 4);
                    
                    // Parse JSON and process FCFS
                    std::vector<Process> processes = parseJSON(jsonData);
                    
                    if (!processes.empty()) {
                        // Calculate FCFS
                        findFCFS(processes);
                        
                        // Generate response JSON
                        std::string responseJson = generateJSON(processes);
                        
                        // Prepare HTTP response with CORS headers
                        std::stringstream response;
                        response << "HTTP/1.1 200 OK\r\n";
                        response << "Content-Type: application/json\r\n";
                        response << "Access-Control-Allow-Origin: *\r\n";
                        response << "Access-Control-Allow-Methods: POST, OPTIONS\r\n";
                        response << "Access-Control-Allow-Headers: Content-Type\r\n";
                        response << "Content-Length: " << responseJson.length() << "\r\n";
                        response << "\r\n";
                        response << responseJson;
                        
                        // Send the response
                        send(clientSocket, response.str().c_str(), response.str().length(), 0);
                    } else {
                        // No valid processes found
                        std::string errorMsg = "{\"error\":\"No valid processes provided\"}";
                        
                        std::stringstream response;
                        response << "HTTP/1.1 400 Bad Request\r\n";
                        response << "Content-Type: application/json\r\n";
                        response << "Access-Control-Allow-Origin: *\r\n";
                        response << "Access-Control-Allow-Methods: POST, OPTIONS\r\n";
                        response << "Access-Control-Allow-Headers: Content-Type\r\n";
                        response << "Content-Length: " << errorMsg.length() << "\r\n";
                        response << "\r\n";
                        response << errorMsg;
                        
                        send(clientSocket, response.str().c_str(), response.str().length(), 0);
                    }
                }
            }
            // Handle OPTIONS request (CORS preflight)
            else if (request.find("OPTIONS /fcfs HTTP/1.1") != std::string::npos) {
                std::stringstream response;
                response << "HTTP/1.1 200 OK\r\n";
                response << "Access-Control-Allow-Origin: *\r\n";
                response << "Access-Control-Allow-Methods: POST, OPTIONS\r\n";
                response << "Access-Control-Allow-Headers: Content-Type\r\n";
                response << "Content-Length: 0\r\n";
                response << "\r\n";
                
                send(clientSocket, response.str().c_str(), response.str().length(), 0);
            }
            // Could handle other routes here if needed
        }
        
        // Close the client socket
        closesocket(clientSocket);
    }

    // Cleanup
    closesocket(serverSocket);
    WSACleanup();
    
    return 0;
}
