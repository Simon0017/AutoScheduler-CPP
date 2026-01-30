
# Auto Scheduler C++

A high-performance task scheduling library written in C++.

![Clock](https://images.unsplash.com/photo-1577436705536-4f2b677b1639?q=80&w=812&auto=format&fit=crop&ixlib=rb-4.1.0&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D)

# Smart Scheduler (C++)

Smart Scheduler is a simple C++ project that automatically generates lesson schedules based on time constraints and preferences.  
It uses JSON for input and output and applies a basic scoring algorithm to assign lessons to optimal time slots.

This project was built as a hobby/learning project to explore scheduling logic, time handling in C++, and JSON-based data processing.

---

## Features

- Generate lesson schedules automatically
- JSON-based input and output
- Time slot generation within a date range
- Basic scoring algorithm for optimal slot selection
- Avoids slot conflicts
- Orders final schedule by date and time

---

## Project Structure
```
smart-scheduler/
│
├── src/
│ ├── main.cpp # Main program
│ ├── scheduler_algo.cpp # Scheduler algorithm class
│
├── data/
│ ├── mock_data.json # Input data
│ ├── generated_schedule.json # Output schedule
│
├── include/
│ └── json.hpp # nlohmann JSON library
│ └── scheduler_algo.hpp # scheduler algorithm class declararion
|
└── README.md
└── .gitignore
```

## Getting Started
### How It Works

1. Load lessons and preferences from a JSON file.
2. Generate available time slots between the start and end dates.
3. Prioritize lessons.
4. Assign lessons to the best available time slots.
5. Save the generated schedule to a JSON file.

### Example input file
```json
{
  "startDate": "2026-01-30",
  "endDate": "2026-02-05",
  "dailyHours": {
    "start": "08:00",
    "end": "17:00"
  },
  "workingDays": ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday"],
  "breakBetweenLessons": 10,
  "room": "Lab A",
  "lessons": [
    {
      "name": "Mathematics",
      "totalHours": 4,
      "sessionDuration": 60
    }
  ]
}
```

### Prerequisites

- C++17 or later
- A modern compiler (GCC,G++, Clang, or MSVC)
- nlohmann/json library

### Building

```
ctrl+shift+p
ctrl+shift+b
```

### Usage

```cpp
#include "scheduler.h"

int main() {
    // intialize scheduler
    scheduler_algorithm scheduler(lessons,preferences);
    cout << "Generating schedule...\n";
    vector<json> schedule = scheduler.generate_schedule();

    cout << "Generated Schedule saved:\n";
}
```