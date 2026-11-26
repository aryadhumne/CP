#ifndef GAME2048_H
#define GAME2048_H

class Game2048 {
public:
    Game2048();
    int play(); // returns score or -1 on exit
private:
    static const int SIZE = 4;
    int board[SIZE][SIZE];
    void initBoard();
    void displayBoard() const;
    bool move(char direction);
    bool canMove() const;
    void addRandomTile();
    bool slideAndCombine(int line[SIZE]);
};

#endif // GAME2048_H