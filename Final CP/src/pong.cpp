#include "pong.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

static void gotoXY(int x, int y)
{
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

static void printAt(int x, int y, const string &s)
{
    gotoXY(x, y);
    cout << s;
}

Pong::Pong()
{
    srand((unsigned)time(0));
    width    = 60;
    height   = 24;
    paddleH  = 4;
    maxScore = 5;
    score1   = 0;
    score2   = 0;

    ballX  = width / 2;
    ballY  = height / 2;
    ballDX = (rand() % 2 == 0) ? 1 : -1;
    ballDY = (rand() % 2 == 0) ? 1 : -1;

    paddle1Y = height / 2 - paddleH / 2;
    paddle2Y = height / 2 - paddleH / 2;
}

Pong::~Pong()
{
    cout << "\n Pong ended!\n";
}

void Pong::draw()
{
    // Top & bottom border
    gotoXY(0, 0);
    for (int x = 0; x < width; x++) cout << '-';
    gotoXY(0, height);
    for (int x = 0; x < width; x++) cout << '-';

    // Middle dotted line
    for (int y = 1; y < height; y++)
        printAt(width / 2, y, (y % 2 == 0) ? "|" : " ");

    // Paddles
    for (int i = 0; i < paddleH; i++)
    {
        printAt(1,         paddle1Y + i, "|");
        printAt(width - 2, paddle2Y + i, "|");
    }

    // Ball
    printAt(ballX, ballY, "O");

    // Score
    printAt(width / 2 - 8, 0, " P1: " + to_string(score1) +
            "   P2: " + to_string(score2) + " ");

    // Controls hint
    printAt(0, height + 1,
        " P1: W/S  |  P2: UP/DOWN  |  ESC: quit  |  First to " +
        to_string(maxScore) + " wins!  ");
}

void Pong::moveBall()
{
    // Erase old ball
    printAt(ballX, ballY, " ");

    ballX += ballDX;
    ballY += ballDY;

    // Top / bottom bounce
    if (ballY <= 1)       { ballY = 1;        ballDY = 1;  }
    if (ballY >= height - 1) { ballY = height - 1; ballDY = -1; }
}

bool Pong::checkPaddleHit()
{
    // Left paddle (player 1) at x=2
    if (ballX == 2 && ballY >= paddle1Y && ballY < paddle1Y + paddleH)
    {
        ballDX = 1;
        // Add slight angle based on where ball hits paddle
        int rel = ballY - paddle1Y;
        if (rel == 0 || rel == paddleH - 1) ballDY = (ballDY > 0) ? 1 : -1;
        return true;
    }

    // Right paddle (player 2) at x=width-3
    if (ballX == width - 3 && ballY >= paddle2Y && ballY < paddle2Y + paddleH)
    {
        ballDX = -1;
        int rel = ballY - paddle2Y;
        if (rel == 0 || rel == paddleH - 1) ballDY = (ballDY > 0) ? 1 : -1;
        return true;
    }

    // Ball missed left paddle — P2 scores
    if (ballX <= 0)
    {
        score2++;
        ballX  = width / 2;
        ballY  = height / 2;
        ballDX = -1;
        ballDY = (rand() % 2 == 0) ? 1 : -1;
        Sleep(600);
        return false;
    }

    // Ball missed right paddle — P1 scores
    if (ballX >= width - 1)
    {
        score1++;
        ballX  = width / 2;
        ballY  = height / 2;
        ballDX = 1;
        ballDY = (rand() % 2 == 0) ? 1 : -1;
        Sleep(600);
        return false;
    }

    return true;
}

int Pong::play()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Hide cursor
    CONSOLE_CURSOR_INFO cursor;
    GetConsoleCursorInfo(hOut, &cursor);
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursor);

    system("cls");

    while (score1 < maxScore && score2 < maxScore)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;

        // Player 1 controls: W / S
        if ((GetAsyncKeyState('W') & 0x8000) && paddle1Y > 1)
            paddle1Y--;
        if ((GetAsyncKeyState('S') & 0x8000) && paddle1Y + paddleH < height)
            paddle1Y++;

        // Player 2 controls: UP / DOWN arrow
        if ((GetAsyncKeyState(VK_UP)   & 0x8000) && paddle2Y > 1)
            paddle2Y--;
        if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && paddle2Y + paddleH < height)
            paddle2Y++;

        moveBall();
        checkPaddleHit();
        draw();

        Sleep(60);
    }

    // Restore cursor
    cursor.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursor);

    system("cls");
    if (score1 >= maxScore)
        cout << "\n *** PLAYER 1 WINS! *** Score: " << score1 << " - " << score2 << "\n";
    else if (score2 >= maxScore)
        cout << "\n *** PLAYER 2 WINS! *** Score: " << score1 << " - " << score2 << "\n";
    else
        cout << "\n Game exited. Score: " << score1 << " - " << score2 << "\n";

    return score1; // return player 1 score
}
