# Operating System Project - First Come First Serve (FCFS) Scheduling

This project implements the First Come First Serve (FCFS) CPU scheduling algorithm with a web interface.

## Project Structure
```
OS project (FCFS) using C++/
├── program.cpp      # Main C++ server implementation
├── program.exe     # Compiled executable
├── index.html      # Web interface
└── output/         # Output directory
```

## Features
- FCFS CPU Scheduling Algorithm implementation
- Web-based user interface
- Real-time process scheduling visualization
- JSON-based data communication

## How to Run
1. Compile the C++ program:
   ```bash
   g++ program.cpp -o program.exe
   ```
2. Run the server:
   ```bash
   ./program.exe
   ```
3. Open `index.html` in your web browser
4. Enter process details and click "Calculate FCFS"

## Technical Details
- The server runs on port 5000
- Uses Windows Sockets (Winsock) for networking
- Implements basic HTTP server functionality
- Supports CORS for web interface communication

## Note about Crow Library
This project uses raw socket programming instead of the Crow library for simplicity and educational purposes. The implementation demonstrates basic networking concepts and FCFS algorithm without external dependencies.

## Learning Resources
- [FCFS Scheduling Algorithm](https://www.geeksforgeeks.org/program-for-fcfs-cpu-scheduling-set-1/)
- [Windows Socket Programming](https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2)