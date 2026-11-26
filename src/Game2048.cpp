#include "Game2048.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iomanip>
#include <cstring>
#include <cctype>

using namespace std;

Game2048::Game2048() {
    initBoard();
    srand(static_cast<unsigned>(time(0)));
    addRandomTile();
    addRandomTile();
}

void Game2048::initBoard() {
    for(int i=0;i<SIZE;++i)
        for(int j=0;j<SIZE;++j)
            board[i][j]=0;
}

void Game2048::displayBoard() const {
    cout << "\n+----+----+----+----+\n";
    for(int i=0;i<SIZE;++i){
        cout << "|";
        for(int j=0;j<SIZE;++j){
            if(board[i][j]==0) cout << setw(4) << "" << "|";
            else cout << setw(4) << board[i][j] << "|";
        }
        cout << "\n+----+----+----+----+\n";
    }
}

bool Game2048::slideAndCombine(int line[SIZE]) {
    bool moved = false;
    vector<int> tmp;
    for(int i=0;i<SIZE;++i) if(line[i]!=0) tmp.push_back(line[i]);
    for(size_t i=0;i+1<tmp.size(); ++i){
        if(tmp[i]==tmp[i+1]){
            tmp[i]*=2;
            tmp.erase(tmp.begin()+i+1);
            moved = true;
        }
    }
    for(int i=0;i<SIZE;++i){
        int val = (i<(int)tmp.size()) ? tmp[i] : 0;
        if(line[i]!=val){ moved = true; line[i]=val; }
    }
    return moved;
}

bool Game2048::move(char direction) {
    bool moved = false;
    if(direction=='w'){
        for(int col=0;col<SIZE;++col){
            int line[SIZE];
            for(int row=0;row<SIZE;++row) line[row]=board[row][col];
            moved |= slideAndCombine(line);
            for(int row=0;row<SIZE;++row) board[row][col]=line[row];
        }
    } else if(direction=='s'){
        for(int col=0;col<SIZE;++col){
            int line[SIZE];
            for(int row=0;row<SIZE;++row) line[SIZE-1-row]=board[row][col];
            moved |= slideAndCombine(line);
            for(int row=0;row<SIZE;++row) board[row][col]=line[SIZE-1-row];
        }
    } else if(direction=='a'){
        for(int row=0;row<SIZE;++row){
            int line[SIZE];
            for(int col=0;col<SIZE;++col) line[col]=board[row][col];
            moved |= slideAndCombine(line);
            for(int col=0;col<SIZE;++col) board[row][col]=line[col];
        }
    } else if(direction=='d'){
        for(int row=0;row<SIZE;++row){
            int line[SIZE];
            for(int col=0;col<SIZE;++col) line[SIZE-1-col]=board[row][col];
            moved |= slideAndCombine(line);
            for(int col=0;col<SIZE;++col) board[row][col]=line[SIZE-1-col];
        }
    }
    if(moved) addRandomTile();
    return moved;
}

bool Game2048::canMove() const {
    for(int i=0;i<SIZE;++i) for(int j=0;j<SIZE;++j) if(board[i][j]==0) return true;
    for(int i=0;i<SIZE;++i) for(int j=0;j<SIZE-1;++j) if(board[i][j]==board[i][j+1]) return true;
    for(int j=0;j<SIZE;++j) for(int i=0;i<SIZE-1;++i) if(board[i][j]==board[i+1][j]) return true;
    return false;
}

void Game2048::addRandomTile(){
    vector<pair<int,int>> empty;
    for(int i=0;i<SIZE;++i) for(int j=0;j<SIZE;++j) if(board[i][j]==0) empty.emplace_back(i,j);
    if(empty.empty()) return;
    auto p = empty[rand()%empty.size()];
    int r = p.first;
    int c = p.second;
    board[r][c] = (rand()%10<9)?2:4;
}

int Game2048::play(){
    cout << "\n=== 2048 ===\nUse W/A/S/D to move.\n";
    char input;
    while(true){
        displayBoard();
        if(!canMove()){
            cout << "Game over! No moves left.\n";
            break;
        }
        cout << "Enter move (W/A/S/D) or Q to quit: ";
        cin >> input;
        if(tolower(input)=='q') return -1;
        if(strchr("wasd", tolower(input))){
            if(!move(tolower(input))) cout << "Invalid move! Try another direction.\n";
        } else {
            cout << "Invalid input.\n";
        }
        for(int i=0;i<SIZE;++i) for(int j=0;j<SIZE;++j) if(board[i][j]==2048){
            cout << "Congratulations! You reached 2048!\n";
            return 2048;
        }
    }
    int score=0; for(int i=0;i<SIZE;++i) for(int j=0;j<SIZE;++j) score+=board[i][j];
    return score;
}
