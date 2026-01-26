
# Auto Scheduler C++

A high-performance task scheduling library written in C++.

![Clock](https://images.unsplash.com/photo-1577436705536-4f2b677b1639?q=80&w=812&auto=format&fit=crop&ixlib=rb-4.1.0&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D)

## Features

- Priority-based scheduling
- Chrono(Modern datetime handling)

## Getting Started

### Prerequisites

- C++17 or later
- A modern compiler (GCC,G++, Clang, or MSVC)

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

## Project Structure

```
├───.vscode/
├───build/
├───data/
├───include/
└───src/
```

## License

See LICENSE file for details.
