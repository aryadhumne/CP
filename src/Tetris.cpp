#include "Tetris.h"
#include <iostream>
#include <windows.h>
#include <algorithm>
using namespace std;

Tetris::Tetris()
{
    score = 0;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int winCols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int winRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int n = min(winRows - 1, winCols / 2);
    n = min(n, min(MAX_ROWS, MAX_COLS / 2));
    rows = n;
    cols = 2 * n;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1)
                board[i][j] = '|';
            else
                board[i][j] = ' ';
        }
    }

    start();
}

Tetris::~Tetris()
{
}

void Tetris::drawBoard()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << board[i][j];
        }
        cout << endl;
    }
}

void Tetris::start()
{
    bool h = true;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int startWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int startHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int width = startWidth;
    int height = startHeight;

    while (h)
    {
        GetConsoleScreenBufferInfo(
            GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }
        else if (width != startWidth || height != startHeight)
        {
            cout << "Game END" << endl;
            return;
        }
        else
        {
            Sleep(100);
            system("cls");
            drawBoard();
            drawTetromino(0, 0, 5, 5);
            Sleep(100);
            // system("cls");
            h = false;
        }
    }
}

void Tetris::drawTetromino(int tetrominoIndex, int rotation, int x, int y)
{
}