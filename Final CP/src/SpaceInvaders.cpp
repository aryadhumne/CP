
#include "SpaceInvaders.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;
const int ENEMY_ROWS = 3;
const int ENEMY_COLS = 8;

int SpaceInvaders::play()
{
    int playerX = WIDTH / 2;
    int playerY = HEIGHT - 2;

    int bulletX = -1;
    int bulletY = -1;

    bool enemies[ENEMY_ROWS][ENEMY_COLS];

    // Initialize enemies
    for (int i = 0; i < ENEMY_ROWS; i++)
    {
        for (int j = 0; j < ENEMY_COLS; j++)
        {
            enemies[i][j] = true;
        }
    }

    int enemyX = 3;
    int enemyY = 2;
    int enemyDirection = 1;

    int score = 0;
    int totalEnemies = ENEMY_ROWS * ENEMY_COLS;
    int frame = 0;

    // Console settings
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursorInfo);

    while (totalEnemies > 0)
    {
        // Keyboard controls
        if (_kbhit())
        {
            char key = _getch();

            if (key == 'a' || key == 'A')
                playerX--;

            if (key == 'd' || key == 'D')
                playerX++;

            if (key == ' ')
            {
                if (bulletY == -1)
                {
                    bulletX = playerX;
                    bulletY = playerY - 1;
                }
            }

            if (key == 'q' || key == 'Q')
                return score;
        }

        // Keep player inside screen
        if (playerX < 0)
            playerX = 0;

        if (playerX >= WIDTH)
            playerX = WIDTH - 1;

        // Move bullet
        if (bulletY != -1)
        {
            bulletY--;

            // Check bullet collision
            for (int i = 0; i < ENEMY_ROWS; i++)
            {
                for (int j = 0; j < ENEMY_COLS; j++)
                {
                    int x = enemyX + j * 4;
                    int y = enemyY + i * 2;

                    if (enemies[i][j] &&
                        bulletX == x &&
                        bulletY == y)
                    {
                        enemies[i][j] = false;
                        bulletY = -1;
                        score += 10;
                        totalEnemies--;
                        break;
                    }
                }

                if (bulletY == -1)
                    break;
            }

            if (bulletY < 0)
                bulletY = -1;
        }

        // Move enemies
        frame++;

        if (frame % 5 == 0)
        {
            enemyX += enemyDirection;

            if (enemyX <= 1 || enemyX >= WIDTH - ENEMY_COLS * 4)
            {
                enemyDirection = -enemyDirection;
                enemyY++;
            }
        }

        // Check if enemies reach player
        bool gameOver = false;

        for (int i = 0; i < ENEMY_ROWS; i++)
        {
            for (int j = 0; j < ENEMY_COLS; j++)
            {
                if (enemies[i][j] &&
                    enemyY + i * 2 >= playerY - 1)
                {
                    gameOver = true;
                    break;
                }
            }

            if (gameOver)
                break;
        }

        if (gameOver)
            break;

        // Draw game at the same console position
        COORD position = {0, 0};
        SetConsoleCursorPosition(console, position);

        cout << "========== SPACE INVADERS ==========\n";
        cout << "Score: " << score << "\n";
        cout << "Controls: A = Left | D = Right | SPACE = Shoot | Q = Quit\n\n";

        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                bool printed = false;

                // Draw player
                if (x == playerX && y == playerY)
                {
                    cout << "^";
                    printed = true;
                }

                // Draw bullet
                if (!printed && x == bulletX && y == bulletY)
                {
                    cout << "|";
                    printed = true;
                }

                // Draw enemies
                if (!printed)
                {
                    for (int i = 0; i < ENEMY_ROWS; i++)
                    {
                        for (int j = 0; j < ENEMY_COLS; j++)
                        {
                            int ex = enemyX + j * 4;
                            int ey = enemyY + i * 2;

                            if (enemies[i][j] && x == ex && y == ey)
                            {
                                cout << "M";
                                printed = true;
                                break;
                            }
                        }

                        if (printed)
                            break;
                    }
                }

                if (!printed)
                    cout << " ";
            }

            cout << "\n";
        }

        Sleep(60);
    }

    // Display final result
    SetConsoleCursorPosition(console, {0, HEIGHT + 3});

    if (totalEnemies == 0)
        cout << "\nYOU WIN! All enemies destroyed!\n";
    else
        cout << "\nGAME OVER! Enemies reached your spaceship!\n";

    cout << "Final Score: " << score << "\n";

    // Restore cursor visibility
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(console, &cursorInfo);

    return score;
}
