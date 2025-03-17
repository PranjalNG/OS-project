# First Come First Serve (FCFS) CPU Scheduling Algorithm Implementation

## 🚀 Project Overview
A sophisticated implementation of the First Come First Serve (FCFS) CPU scheduling algorithm with a modern web interface. This project demonstrates efficient process management and real-time scheduling visualization.

## 🛠️ Technology Stack
- **Backend**: C++ with Crow Framework
- **Frontend**: HTML5, CSS3, JavaScript
- **Build System**: CMake
- **Version Control**: Git
- **Documentation**: Markdown

## 📋 Features
- **Interactive Process Management**
  - Dynamic process addition and removal
  - Real-time scheduling visualization
  - Intuitive user interface
- **Advanced Scheduling Metrics**
  - Completion Time calculation
  - Turnaround Time analysis
  - Waiting Time optimization
- **Real-time Visualization**
  - Gantt chart representation
  - Process timeline display
  - Performance metrics dashboard
- **Error Handling & Validation**
  - Input validation
  - Process limit enforcement
  - Error message display

## 🏗️ System Architecture
```
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│   Frontend      │     │   Backend       │     │   FCFS          │
│  (HTML/CSS/JS)  │◄───►│  (C++/Crow)    │◄───►│  Algorithm     │
└─────────────────┘     └─────────────────┘     └─────────────────┘
```

## 🚀 Getting Started

### Prerequisites
- C++ compiler (C++17 or later)
- CMake (version 3.10 or later)
- Git
- Modern web browser

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/PranjalNG/OS-project.git
   cd OS-project
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. Run the application:
   ```bash
   ./fcfs_scheduler
   ```

4. Access the web interface:
   - Open your browser
   - Navigate to `http://localhost:5000`

## 💻 Usage Guide

### Adding Processes
1. Enter process details:
   - Process ID (unique identifier)
   - Arrival Time (when process enters the system)
   - Burst Time (CPU time required)

2. Click "Add Process" to include in scheduling queue

### Viewing Results
- **Gantt Chart**: Visual representation of process execution
- **Timeline**: Detailed process execution sequence
- **Metrics**: Performance analysis including:
  - Completion Time
  - Turnaround Time
  - Waiting Time

## 📊 Performance Metrics

### Key Calculations
1. **Completion Time (CT)**
   - Time when process finishes execution
   - Formula: CT = Previous Process CT + Current Process BT

2. **Turnaround Time (TAT)**
   - Total time from arrival to completion
   - Formula: TAT = CT - AT

3. **Waiting Time (WT)**
   - Time process waits in ready queue
   - Formula: WT = TAT - BT

### Average Metrics
- Average Turnaround Time = Σ(TAT) / n
- Average Waiting Time = Σ(WT) / n

## 🔍 Technical Implementation

### Backend Architecture
```cpp
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};
```

### Frontend Components
- Process input form
- Real-time visualization
- Results display panel
- Error handling interface

## 🧪 Testing
- Unit tests for FCFS algorithm
- Integration tests for API endpoints
- UI/UX testing for web interface
- Performance benchmarking

## 📈 Future Enhancements
1. **Algorithm Extensions**
   - Priority scheduling
   - Round Robin implementation
   - Multi-level queue scheduling

2. **UI Improvements**
   - Dark mode support
   - Responsive design optimization
   - Interactive animations

3. **Performance Optimizations**
   - Caching mechanisms
   - Load balancing
   - Real-time updates

## 👥 Team
- **Project Lead**: [Your Name]
- **Backend Developer**: [Team Member 2]
- **Frontend Developer**: [Team Member 3]
- **Testing & Documentation**: [Team Member 4]

## 📝 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🤝 Contributing
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📞 Support
For support, email [your.email@example.com] or create an issue in the repository.

## 🙏 Acknowledgments
- Operating Systems course faculty
- Open-source community
- Project contributors

---
Made with ❤️ by [Your Team Name]