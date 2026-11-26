#include "Sudoku.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <cctype>

using namespace std;

Sudoku::Sudoku() {
    // generate a full solution then puzzle when play() is called
}

void Sudoku::printBoard() const {
    for(int i=0;i<9;++i){
        if(i%3==0) cout << "---------------------------\n";
        for(int j=0;j<9;++j){
            if(j%3==0) cout << "| ";
            if(board[i][j]==0) cout << ". ";
            else cout << board[i][j] << ' ';
        }
        cout << "|\n";
    }
    cout << "---------------------------\n";
}

bool Sudoku::isValidMove(int row, int col, int num) const {
    if(board[row][col]!=0) return false;
    for(int i=0;i<9;++i){
        if(board[row][i]==num || board[i][col]==num) return false;
    }
    int boxRow=row/3*3, boxCol=col/3*3;
    for(int i=0;i<3;++i) for(int j=0;j<3;++j)
        if(board[boxRow+i][boxCol+j]==num) return false;
    return true;
}

bool Sudoku::solve(){
    for(int r=0;r<9;++r){
        for(int c=0;c<9;++c){
            if(board[r][c]==0){
                for(int n=1;n<=9;++n){
                    if(isValidMove(r,c,n)){
                        board[r][c]=n;
                        if(solve()) return true;
                        board[r][c]=0;
                    }
                }
                return false;
            }
        }
    }
    return true; // solved
}

void Sudoku::generatePuzzle(Difficulty level){
    // start with empty board and fill a solved board
    for(int i=0;i<9;++i) for(int j=0;j<9;++j) board[i][j]=0;
    solve(); // board now holds a complete solution
    // copy solution
    for(int i=0;i<9;++i) for(int j=0;j<9;++j) solution[i][j]=board[i][j];
    // determine number of cells to remove based on difficulty
    int removals = (level==Easy)?30:(level==Medium)?45:55;
    random_device rd; mt19937 gen(rd());
    uniform_int_distribution<> dis(0,8);
    int removed=0;
    while(removed<removals){
        int r=dis(gen), c=dis(gen);
        if(board[r][c]!=0){
            board[r][c]=0;
            ++removed;
        }
    }
}

Sudoku::Difficulty Sudoku::selectDifficulty(){
    cout << "Select difficulty: 1. Easy 2. Medium 3. Hard : ";
    int choice; cin >> choice;
    switch(choice){
        case 1: return Easy;
        case 2: return Medium;
        default: return Hard;
    }
}

void Sudoku::userInput(){
    while(true){
        printBoard();
        cout << "Enter row (1-9) col (1-9) number (1-9), or 0 0 0 to finish: ";
        int r,c,n; cin>>r>>c>>n;
        if(r==0 && c==0 && n==0) break;
        if(r<1||r>9||c<1||c>9||n<1||n>9){
            cout << "Invalid range. Try again.\n";
            continue;
        }
        if(!isValidMove(r-1,c-1,n)){
            cout << "Move violates Sudoku rules. Try again.\n";
            continue;
        }
        board[r-1][c-1]=n;
        if(isComplete()) break;
    }
}

bool Sudoku::isComplete() const{
    for(int i=0;i<9;++i) for(int j=0;j<9;++j) if(board[i][j]==0) return false;
    return true;
}

bool Sudoku::checkSolution() const{
    // compare with stored solution
    for(int i=0;i<9;++i) for(int j=0;j<9;++j) if(board[i][j]!=solution[i][j]) return false;
    return true;
}

int Sudoku::play(){
    cout << "\n=== SUDOKU ===\n";
    Difficulty diff = selectDifficulty();
    generatePuzzle(diff);
    userInput();
    if(checkSolution()){
        cout << "Congratulations! Puzzle solved correctly.\n";
        return 0;
    } else {
        cout << "Incorrect solution. Better luck next time.\n";
        return -1;
    }
}
