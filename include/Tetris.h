#ifndef Tetris_h
#define Tetris_h
#include <string>

class Tetromino
{
    std::string shape[4][4];

public:
    Tetromino();
};

class Tetris
{
    static constexpr int MAX_ROWS = 100;
    static constexpr int MAX_COLS = 200;
    char board[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
    int score;
    Tetromino tetrominoes[5];

public:
    Tetris();
    ~Tetris();
    void drawBoard();
    void start();
    void drawTetromino(int tetrominoIndex, int rotation, int x, int y);
    bool checkCollision(int tetrominoIndex, int rotation, int x, int y);
    void placeTetromino(int tetrominoIndex, int rotation, int x, int y);
    void clearLines();
};

#endif // Tetris_h