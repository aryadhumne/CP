# OOP Multi-Game Engine

A C++17 mini-game project built using SFML 3.0.2. It includes a graphical menu and two simple games.

## Games

1. Snake
2. Tic-Tac-Toe

## Controls

### Main Menu
- Press `1` to play Snake
- Press `2` to play Tic-Tac-Toe
- Press `ESC` to quit

### Snake
- `W` - Move up
- `A` - Move left
- `S` - Move down
- `D` - Move right
- `R` - Restart after Game Over
- `ESC` - Return to menu

### Tic-Tac-Toe
- Left mouse click - Place X or O
- `R` - Restart the game
- `ESC` - Return to menu

## Features

- Graphical game window
- Simple main menu
- Snake movement and food collection
- Score tracking and collision detection
- Two-player Tic-Tac-Toe
- Win and draw detection
- Restart support

## Requirements

- C++17 compiler
- CMake
- Ninja
- SFML 3.0.2
- MSYS2 UCRT64 on Windows

## Build

Configure the project:

```bash
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe
```

Build the project:

```bash
cmake --build build
```

## Run

On Windows, run the executable from the project folder:

```bash
.\build\games.exe
```

## Project Structure

```text
oops/
├── main.cpp
├── CMakeLists.txt
├── README.md
└── build/
```

## License

This project is available for learning and personal use.
