# HookFinder

HookFinder is a lightweight tool designed to inject a DLL into a running process to monitor for Ring 3 hooks. This project relies on Microsoft Detours for DLL injection and hook detection.

## Prerequisites

Before you can build and run HookMonitor, ensure that you have the following:

- **Microsoft Detours**: Download and install from [Microsoft Detours](https://www.microsoft.com/en-us/research/project/detours/)
- **Windows Operating System**: This project is developed and tested on Windows.

## Getting Started

### Building the Project

1. **Clone the Repository**:
    ```sh
    git clone git@github.com:pulpocaminante/HookFinder.git    
    cd HookFinder
    ```

2. **Set Up Microsoft Detours**:
    - Follow the installation instructions for Microsoft Detours from the [official site](https://www.microsoft.com/en-us/research/project/detours/).
    - Make sure to include the Detours library in your project. Adjust the project settings to link against `detours.lib`.

3. **Build the Project**:
    - Open the project in your IDE (e.g., Visual Studio).
    - Build the solution. Ensure there are no errors.

### Running HookFinder

1. **Inject the DLL**:
    - Use a DLL injection tool

2. **??? Profit**
