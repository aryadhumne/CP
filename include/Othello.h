#ifndef OTHELLO_H
#define OTHELLO_H

#include <vector>
#include <utility>
#include <cstring>

class Othello {
public:
    Othello();
    int play(); // returns score (difference in disc count) or -1 on exit
private:
    static const int SIZE = 8;
    char board[SIZE][SIZE];
    void initBoard();
    void displayBoard() const;
    bool isValidMove(int row, int col, char player) const;
    void applyMove(int row, int col, char player);
    void flipLine(int row, int col, int dRow, int dCol, char player);
    std::vector<std::pair<int,int>> getAllValidMoves(char player) const;
    bool hasAnyMove(char player) const;
    int countDiscs(char player) const;
    char opponent(char player) const { return (player == 'B') ? 'W' : 'B'; }
    int evaluateScore() const; // B - W
};

#endif // OTHELLO_H