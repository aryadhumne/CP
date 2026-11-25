
#include "Breakout.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

int Breakout::play()
{
    int paddleX = WIDTH / 2 - 3;

    int ballX = WIDTH / 2;
    int ballY = HEIGHT - 4;

    int ballDX = 1;
    int ballDY = -1;

    int score = 0;
    int lives = 3;
    int bricksRemaining = 50;

    bool bricks[5][10];

    // Initialize all bricks
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            bricks[i][j] = true;
        }
    }
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

CONSOLE_CURSOR_INFO cursorInfo;
GetConsoleCursorInfo(console, &cursorInfo);
cursorInfo.bVisible = FALSE;
SetConsoleCursorInfo(console, &cursorInfo);
    while (lives > 0 && bricksRemaining > 0)
    {
        COORD position = {0, 0};
        SetConsoleCursorPosition(console, position);

        cout << "========== BREAKOUT ==========\n";
        cout << "Score: " << score << "   Lives: " << lives << "\n\n";

        // Draw game board
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                bool printed = false;

                // Draw bricks
                if (y < 5 && x >= 5 && x < 35)
                {
                    int row = y;
                    int col = (x - 5) / 3;

                    if (col < 10 && bricks[row][col])
                    {
                        cout << "#";
                        printed = true;
                    }
                }

                // Draw ball
                if (!printed && x == ballX && y == ballY)
                {
                    cout << "O";
                    printed = true;
                }

                // Draw paddle
                if (!printed && y == HEIGHT - 2 &&
                    x >= paddleX && x < paddleX + 7)
                {
                    cout << "=";
                    printed = true;
                }

                if (!printed)
                    cout << " ";
            }

            cout << "\n";
        }

        cout << "\nControls: A = Left | D = Right | Q = Quit\n";

        // Keyboard input
        if (_kbhit())
        {
            char key = _getch();

            if (key == 'a' || key == 'A')
                paddleX--;

            if (key == 'd' || key == 'D')
                paddleX++;

            if (key == 'q' || key == 'Q')
                return score;
        }

        // Keep paddle inside the board
        if (paddleX < 0)
            paddleX = 0;

        if (paddleX > WIDTH - 7)
            paddleX = WIDTH - 7;

        // Move ball
        ballX += ballDX;
        ballY += ballDY;

        // Wall collision
        if (ballX <= 0 || ballX >= WIDTH - 1)
            ballDX = -ballDX;

        if (ballY <= 0)
            ballDY = 1;

        // Brick collision
        if (ballY < 5 && ballX >= 5 && ballX < 35)
        {
            int row = ballY;
            int col = (ballX - 5) / 3;

            if (col < 10 && bricks[row][col])
            {
                bricks[row][col] = false;
                ballDY = -ballDY;
                score += 10;
                bricksRemaining--;
            }
        }

        // Paddle collision
        if (ballY == HEIGHT - 3 &&
            ballX >= paddleX && ballX < paddleX + 7)
        {
            ballDY = -1;
        }

        // Ball missed
        if (ballY >= HEIGHT - 1)
        {
            lives--;

            ballX = WIDTH / 2;
            ballY = HEIGHT - 4;

            ballDX = 1;
            ballDY = -1;
        }

        Sleep(100);
    }

   if (bricksRemaining == 0)
{
    cout << "\nCongratulations! YOU WIN!\n";
}
else
{
    cout << "\nGame Over! Final Score: " << score << "\n";
}

cout << "Final Score: " << score << "\n";

return score;
}
