#include "snake.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

static void gotoxy(int x, int y)
{
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

Snake::Snake()
{
    srand((unsigned)time(0));
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top - 2;
    if (width  > 60) width  = 60;
    if (height > 25) height = 25;

    dirX = 1; dirY = 0;
    score = 0;

    // Start snake in the middle of the board
    int startX = width / 2;
    int startY = height / 2;
    body.push_back({startX,     startY});
    body.push_back({startX - 1, startY});
    body.push_back({startX - 2, startY});

    generateFood();
}

Snake::~Snake()
{
    cout << "\n Snake game ended!\n";
}

void Snake::generateFood()
{
    bool onBody;
    do
    {
        onBody = false;
        foodX = 1 + rand() % (width  - 2);
        foodY = 1 + rand() % (height - 2);
        for (auto &p : body)
            if (p.first == foodX && p.second == foodY)
                onBody = true;
    } while (onBody);
}

bool Snake::isOnBody(int x, int y)
{
    for (auto &p : body)
        if (p.first == x && p.second == y)
            return true;
    return false;
}

int Snake::play()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Hide cursor
    CONSOLE_CURSOR_INFO cursor;
    GetConsoleCursorInfo(hOut, &cursor);
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursor);

    system("cls");

    // Draw border
    for (int x = 0; x < width; x++)
    {
        gotoxy(x, 0);          cout << '#';
        gotoxy(x, height - 1); cout << '#';
    }
    for (int y = 0; y < height; y++)
    {
        gotoxy(0,         y); cout << '#';
        gotoxy(width - 1, y); cout << '#';
    }

    // Draw initial snake
    gotoxy(body[0].first, body[0].second); cout << 'O';
    for (int i = 1; i < (int)body.size(); i++)
    {
        gotoxy(body[i].first, body[i].second); cout << 'o';
    }

    // Draw food
    gotoxy(foodX, foodY); cout << '*';

    // Draw initial score
    gotoxy(0, height);
    cout << "Score: " << score << "   |  Arrow keys to move  |  ESC to quit";

    bool gameOver = false;

    while (!gameOver)
    {
        Sleep(120);

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;

        // Read direction input (prevent reversing)
        if ((GetAsyncKeyState(VK_LEFT)  & 0x8000) && dirX != 1)  { dirX = -1; dirY =  0; }
        if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && dirX != -1) { dirX =  1; dirY =  0; }
        if ((GetAsyncKeyState(VK_UP)    & 0x8000) && dirY != 1)  { dirX =  0; dirY = -1; }
        if ((GetAsyncKeyState(VK_DOWN)  & 0x8000) && dirY != -1) { dirX =  0; dirY =  1; }

        // Calculate new head position
        int newX = body[0].first  + dirX;
        int newY = body[0].second + dirY;

        // Wall collision
        if (newX <= 0 || newX >= width - 1 || newY <= 0 || newY >= height - 1)
        {
            gameOver = true;
            break;
        }

        // Self collision
        for (auto &p : body)
        {
            if (p.first == newX && p.second == newY)
            {
                gameOver = true;
                break;
            }
        }
        if (gameOver) break;

        bool eating = (newX == foodX && newY == foodY);

        if (!eating)
        {
            // Erase tail from screen
            gotoxy(body.back().first, body.back().second);
            cout << ' ';
            body.pop_back();
        }
        else
        {
            // Ate food: grow, generate new food, update score
            score += 10;
            generateFood();
            gotoxy(foodX, foodY); cout << '*';
            gotoxy(0, height);
            cout << "Score: " << score << "   ";
        }

        // Convert old head to body segment
        gotoxy(body[0].first, body[0].second); cout << 'o';

        // Insert new head
        body.insert(body.begin(), {newX, newY});
        gotoxy(newX, newY); cout << 'O';
    }

    // Restore cursor
    cursor.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursor);

    gotoxy(0, height + 1);
    cout << " GAME OVER!  Final Score: " << score << "\n";
    Sleep(1500);

    return score;
}
