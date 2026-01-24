
# Auto Scheduler C++

A high-performance task scheduling library written in C++.

## Features

- Priority-based scheduling
- Chrono(Modern datetime handling)

## Getting Started

### Prerequisites

- C++17 or later
- A modern compiler (GCC, Clang, or MSVC)

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
