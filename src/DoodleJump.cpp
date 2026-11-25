
#include "DoodleJump.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;
const int PLATFORM_COUNT = 10;

int DoodleJump::play()
{
    srand((unsigned)time(0));

    int playerX = WIDTH / 2;
    double playerY = 16;
    double velocity = -2.5;

    double camera = 0;
    int score = 0;

    int platformX[PLATFORM_COUNT];
    double platformY[PLATFORM_COUNT];

    // Generate platforms
    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        platformX[i] = rand() % (WIDTH - 6);
        platformY[i] = 18 - i * 3;
    }

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);

    while (true)
    {
        double previousY = playerY;

        // Keyboard controls
        if (_kbhit())
        {
            char key = _getch();

            if (key == 'a' || key == 'A')
                playerX--;

            if (key == 'd' || key == 'D')
                playerX++;

            if (key == 'q' || key == 'Q')
                return score;
        }

        // Keep player inside screen
        if (playerX < 0)
            playerX = 0;

        if (playerX >= WIDTH)
            playerX = WIDTH - 1;

        // Apply gravity
        velocity += 0.15;
        playerY += velocity;

        // Platform collision while falling
        if (velocity > 0)
        {
            for (int i = 0; i < PLATFORM_COUNT; i++)
            {
                if (previousY < platformY[i] &&
                    playerY >= platformY[i] &&
                    playerX >= platformX[i] &&
                    playerX < platformX[i] + 6)
                {
                    velocity = -2.5;
                    break;
                }
            }
        }

        // Move camera upward as player climbs
        if (playerY < camera + 7)
        {
            camera = playerY - 7;
        }

        // Generate new platforms above the screen
        for (int i = 0; i < PLATFORM_COUNT; i++)
        {
            if (platformY[i] > camera + HEIGHT)
            {
                double highest = platformY[0];

                for (int j = 1; j < PLATFORM_COUNT; j++)
                {
                    if (platformY[j] < highest)
                        highest = platformY[j];
                }

                platformY[i] = highest - 3;
                platformX[i] = rand() % (WIDTH - 6);
            }
        }

        // Update score
        int currentScore = (int)(-camera * 10);

        if (currentScore > score)
            score = currentScore;

        // Game over if player falls below screen
        if (playerY - camera >= HEIGHT)
            break;

        // Draw game
        COORD position = {0, 0};
        SetConsoleCursorPosition(console, position);

        cout << "========== DOODLE JUMP ==========\n";
        cout << "Score: " << score << "   Controls: A / D   Q: Quit\n";

        for (int y = 0; y < HEIGHT - 2; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                bool printed = false;

                // Draw player
                if (x == playerX && y == (int)(playerY - camera))
                {
                    cout << "D";
                    printed = true;
                }

                // Draw platforms
                if (!printed)
                {
                    for (int i = 0; i < PLATFORM_COUNT; i++)
                    {
                        int screenY = (int)(platformY[i] - camera);

                        if (y == screenY &&
                            x >= platformX[i] &&
                            x < platformX[i] + 6)
                        {
                            cout << "=";
                            printed = true;
                            break;
                        }
                    }
                }

                if (!printed)
                    cout << " ";
            }

            cout << "\n";
        }

        Sleep(70);
    }

    // Show final result
    cout << "\nGAME OVER!\n";
    cout << "Final Score: " << score << "\n";

    return score;
}
