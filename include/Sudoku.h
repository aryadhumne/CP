#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
#include <string>

class Sudoku {
public:
    Sudoku();
    int play(); // returns score (0 for success, -1 for exit)
private:
    enum Difficulty { Easy, Medium, Hard };
    int board[9][9];
    int solution[9][9];
    void generatePuzzle(Difficulty level);
    void printBoard() const;
    bool isValidMove(int row, int col, int num) const;
    bool solve(); // internal solver for generating solution
    bool isComplete() const;
    void userInput();
    bool checkSolution() const;
    Difficulty selectDifficulty();
};

#endif // SUDOKU_H