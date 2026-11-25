#include "Tetris.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
using namespace std;
const char FILL = (char)219;

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
}

Tetris::~Tetris()
{
}

void Tetris::drawBoard()
{
    COORD home = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), home);

    string frame;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            frame += board[i][j];
        frame += '\n';
    }
    frame += "Score: " + to_string(score) + "      ";

    cout << frame << flush; // one single write = no flicker
}

static bool cellFilled(const string shape[4][4], int rotation, int r, int c)
{
    for (int k = 0; k < rotation; k++)
    {
        int nr = 3 - c;
        int nc = r;
        r = nr;
        c = nc;
    }
    return c < (int)shape[r][0].size() && shape[r][0][c] != ' ';
}

int Tetris::start()
{
    bool h = true, spaceHeld = false;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    int startWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int startHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int width = startWidth, height = startHeight;
    SetConsoleOutputCP(437);
    CONSOLE_CURSOR_INFO cursor;
    GetConsoleCursorInfo(hOut, &cursor);
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursor);
    system("cls");
    srand((unsigned)time(0));
    int index = rand() % 5;
    int y = 1, tick = 0, x = cols / 2 - 2, rotation = 0;
    char backup[MAX_ROWS][MAX_COLS];

    while (h)
    {
        GetConsoleScreenBufferInfo(hOut, &csbi);
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }
        else if (width != startWidth || height != startHeight)
        {
            system("cls");
            cout << "Game END" << endl;
            break;
        }
        else
        {
            Sleep(120);
            if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && !checkCollision(index, rotation, x - 1, y))
                x--;
            if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && !checkCollision(index, rotation, x + 1, y))
                x++;
            if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && !checkCollision(index, rotation, x, y + 1))
                y++;

            bool space = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
            if (space && !spaceHeld && !checkCollision(index, (rotation + 1) % 4, x, y))
                rotation = (rotation + 1) % 4;
            spaceHeld = space;

            tick++;
            if (tick >= 5)
            {
                tick = 0;
                if (!checkCollision(index, rotation, x, y + 1))
                    y++;
            }
            if (checkCollision(index, rotation, x, y + 1))
            {
                placeTetromino(index, rotation, x, y);
                clearLines();

                index = rand() % 5;
                rotation = 0;
                x = cols / 2 - 2;
                y = 1;
                tick = 0;

                if (checkCollision(index, rotation, x, y))
                {
                    drawBoard();
                    COORD pos = {0, (SHORT)rows};
                    SetConsoleCursorPosition(hOut, pos);
                    cout << "Game Over! Score: " << score << "      " << flush;
                    break;
                }
            }

            memcpy(backup, board, sizeof(board));
            drawTetromino(index, rotation, x, y);
            drawBoard();
            memcpy(board, backup, sizeof(board));
        }
    }

    cursor.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursor);
    COORD end = {0, (SHORT)(rows + 1)};
    SetConsoleCursorPosition(hOut, end);
    return score;
}

void Tetris::drawTetromino(int tetrominoIndex, int rotation, int x, int y)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (cellFilled(tetrominoes[tetrominoIndex].shape, rotation, r, c))
            {
                int i = y + r;
                int j = x + c;
                if (i >= 0 && i < rows && j >= 0 && j < cols)
                    board[i][j] = FILL;
            }
        }
    }
}

bool Tetris::checkCollision(int tetrominoIndex, int rotation, int x, int y)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (cellFilled(tetrominoes[tetrominoIndex].shape, rotation, r, c))
            {
                int i = y + r;
                int j = x + c;
                if (i < 0 || i >= rows || j < 0 || j >= cols || board[i][j] != ' ')
                    return true;
            }
        }
    }
    return false;
}

void Tetris::placeTetromino(int tetrominoIndex, int rotation, int x, int y)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (cellFilled(tetrominoes[tetrominoIndex].shape, rotation, r, c))
                board[y + r][x + c] = FILL;
        }
    }
}

void Tetris::clearLines()
{
    for (int i = rows - 2; i >= 1; i--)
    {
        bool full = true;
        for (int j = 1; j < cols - 1; j++)
        {
            if (board[i][j] == ' ')
                full = false;
        }

        if (full)
        {
            for (int k = i; k > 1; k--)
                for (int j = 1; j < cols - 1; j++)
                    board[k][j] = board[k - 1][j];
            for (int j = 1; j < cols - 1; j++)
                board[1][j] = ' ';

            score += 10;
            i++;
        }
    }
}