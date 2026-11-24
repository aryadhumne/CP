# Ultimate Game Console

A C++ terminal-based multi-game engine. All games run directly in the console - no external libraries or CMake required.

## Games

1. Tic Tac Toe (vs Bot or Multiplayer)
2. Rock Paper Scissors
3. Hangman
4. Card Guessing Game
5. Word Scramble
6. Tetris
7. Flappy Bird
8. Snake
9. Pong

## Project Structure

```
oops/
|-- main.cpp              <- Entry point
|-- include/              <- Header files (.h)
|   |-- Auth.h
|   |-- Flappy_Bird.h
|   |-- Tetris.h
|   |-- tic_tac_toe.h
|   |-- hangman.h
|   |-- r_p_s.h
|   |-- scramble.h
|   |-- cardGuessing.h
|   |-- snake.h
|   `-- pong.h
|-- src/                  <- Source files (.cpp)
|   |-- Auth.cpp
|   |-- Flappy_Bird.cpp
|   |-- Tetris.cpp
|   |-- Tetromino.cpp
|   |-- tic_tac_toe.cpp
|   |-- hangman.cpp
|   |-- r_p_s.cpp
|   |-- scramble.cpp
|   |-- cardGuessing.cpp
|   |-- snake.cpp
|   `-- pong.cpp
|-- auth.txt              <- User data (auto-created)
`-- README.md
```

## Requirements

- g++ compiler (MinGW on Windows / GCC on Linux/Mac)
- C++17 or later

## Build & Run

### Windows (Command Prompt or PowerShell)

**Compile:**
```
g++ -Iinclude main.cpp src/Auth.cpp src/Flappy_Bird.cpp src/Tetris.cpp src/Tetromino.cpp src/tic_tac_toe.cpp src/hangman.cpp src/r_p_s.cpp src/scramble.cpp src/cardGuessing.cpp src/snake.cpp src/pong.cpp -o main.exe
```

**Run:**
```
main.exe
```

### Linux / Mac

**Compile:**
```
g++ -Iinclude main.cpp src/Auth.cpp src/Flappy_Bird.cpp src/Tetris.cpp src/Tetromino.cpp src/tic_tac_toe.cpp src/hangman.cpp src/r_p_s.cpp src/scramble.cpp src/cardGuessing.cpp src/snake.cpp src/pong.cpp -o main
```

**Run:**
```
./main
```

## Controls

### Pong
- `W` / `S` - Player 1 paddle (left)
- `UP` / `DOWN` arrow - Player 2 paddle (right)
- First to 5 points wins!
- `ESC` - Exit game

### Snake
- Arrow keys - Move
- `ESC` - Exit game

### Flappy Bird
- `SPACE` - Flap / go up
- `ESC` - Exit game

### Tetris
- `LEFT` / `RIGHT` arrow - Move piece
- `DOWN` arrow - Drop faster
- `SPACE` - Rotate piece
- `ESC` - Exit game

### Tic Tac Toe
- Enter position `1-9` when prompted

### All other games
- Follow on-screen instructions

## Notes

- `auth.txt` stores user login data. Make sure it exists in the same folder as the executable.
- Snake, Flappy Bird, Tetris and Pong use Windows Console API - they work best on Windows.
