# ImGui Loggger

## Development Environment
- **OS**: Windows (MSVC)
- **Tool**: CMake, Visual Studio Code

## Implemented Features
- **Logging System**: Implemented a `Logger` class that outputs color coded logs (INFO, WARN, ERROR, GAME) with timestamps to both an ImGui window and a `log.txt` file. 

## Design
- Logs are displayed both in the terminal (console) and in the in‑game “Game Log” ImGui window.
- Every log call is also appended to `build/Debug/log.txt` for later analysis.
- The GUI log panel always shows all log levels (INFO, WARN, ERROR, GAME), but console output is filtered by a configurable minimum console log level.
- The Game Control panel provides buttons that generate typical gameplay, warning, and error logs, plus a Game Action counter that can later be reused for turn‑based game prototypes.