#include "Othello.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

Othello::Othello() {
    initBoard();
    srand(static_cast<unsigned>(time(0)));
}

void Othello::initBoard() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = '.'; // empty
        }
    }
    // Starting position
    board[3][3] = board[4][4] = 'W';
    board[3][4] = board[4][3] = 'B';
}

void Othello::displayBoard() const {
    cout << "  ";
    for (int c = 0; c < SIZE; ++c) cout << c << ' ';
    cout << '\n';
    for (int i = 0; i < SIZE; ++i) {
        cout << i << ' ';
        for (int j = 0; j < SIZE; ++j) {
            cout << board[i][j] << ' ';
        }
        cout << '\n';
    }
}

bool Othello::isValidMove(int row, int col, char player) const {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) return false;
    if (board[row][col] != '.') return false;
    char opp = opponent(player);
    const int dirs[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
    for (auto &d : dirs) {
        int r = row + d[0];
        int c = col + d[1];
        bool hasOppBetween = false;
        while (r >=0 && r < SIZE && c >=0 && c < SIZE && board[r][c] == opp) {
            hasOppBetween = true;
            r += d[0];
            c += d[1];
        }
        if (hasOppBetween && r >=0 && r < SIZE && c >=0 && c < SIZE && board[r][c] == player) {
            return true;
        }
    }
    return false;
}

vector<pair<int,int>> Othello::getAllValidMoves(char player) const {
    vector<pair<int,int>> moves;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (isValidMove(i,j,player)) moves.emplace_back(i,j);
        }
    }
    return moves;
}

void Othello::flipLine(int row, int col, int dRow, int dCol, char player) {
    char opp = opponent(player);
    int r = row + dRow;
    int c = col + dCol;
    vector<pair<int,int>> toFlip;
    while (r >=0 && r < SIZE && c >=0 && c < SIZE && board[r][c] == opp) {
        toFlip.emplace_back(r,c);
        r += dRow; c += dCol;
    }
    if (r >=0 && r < SIZE && c >=0 && c < SIZE && board[r][c] == player) {
        for (auto &p : toFlip) board[p.first][p.second] = player;
    }
}

void Othello::applyMove(int row, int col, char player) {
    board[row][col] = player;
    const int dirs[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
    for (auto &d : dirs) {
        flipLine(row, col, d[0], d[1], player);
    }
}

bool Othello::hasAnyMove(char player) const {
    return !getAllValidMoves(player).empty();
}

int Othello::countDiscs(char player) const {
    int cnt = 0;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j] == player) ++cnt;
    return cnt;
}

int Othello::evaluateScore() const {
    return countDiscs('B') - countDiscs('W');
}

int Othello::play() {
    cout << "\\n=== OTHELLO ===\\n";
    cout << "Select mode: 1. Player vs Player, 2. Player vs Computer: ";
    int mode; cin >> mode;
    char current = 'B'; // Black starts
    while (hasAnyMove('B') || hasAnyMove('W')) {
        if (!hasAnyMove(current)) {
            current = opponent(current);
            continue;
        }
        displayBoard();
        cout << "Current player: " << (current=='B' ? "Black (B)" : "White (W)") << "\\n";
        int r,c;
        if (mode==2 && current=='W') { // simple AI for White
            auto moves = getAllValidMoves(current);
            auto choice = moves[rand()%moves.size()];
            r = choice.first; c = choice.second;
            cout << "Computer (White) plays: " << r << " " << c << "\\n";
        } else {
            cout << "Enter row and column: ";
            cin >> r >> c;
            while (!isValidMove(r,c,current)) {
                cout << "Invalid move. Try again: ";
                cin >> r >> c;
            }
        }
        applyMove(r,c,current);
        current = opponent(current);
    }
    displayBoard();
    int score = evaluateScore();
    cout << "Game over. Score (Black - White): " << score << "\n";
    return score;
}
