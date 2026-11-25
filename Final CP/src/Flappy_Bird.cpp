#include <windows.h>
#include <iostream>
#include "Flappy_Bird.h"
#include <string>
#include <random>
#include <cwchar>
#include <chrono>
using namespace std;

static void put(const wchar_t *s, int x, int y)
{
    COORD pos = {(SHORT)x, (SHORT)y};
    DWORD written;
    WriteConsoleOutputCharacterW(GetStdHandle(STD_OUTPUT_HANDLE), s, (DWORD)wcslen(s), pos, &written);
}

static bool passGap(const vector<char> &rows, int t, int gap, int height, int birdLen, vector<char> &out)
{
    vector<char> cur = rows;
    for (int f = 0; f < 14; f++)
    {
        bool constrained = f >= 14 - birdLen;
        vector<char> next(height, 0);
        bool any = false;
        for (int y = 0; y < height; y++)
        {
            if (!cur[y])
                continue;
            for (int rise = 0; rise < 2; rise++)
            {
                int ny = y + (rise ? -2 : 1);
                if (ny < 0)
                    ny = 0;
                if (ny >= height)
                    ny = height - 1;
                if (!constrained || (ny >= t && ny < t + gap))
                {
                    next[ny] = 1;
                    any = true;
                }
            }
        }
        if (!any)
            return false;
        cur = next;
    }
    out = cur;
    return true;
}

void FlappyBird::fly(int speed)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    wstring wbird(bird.begin(), bird.end());
    int offset = 0;

    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            return;
        }

        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        if (csbi.srWindow.Right - csbi.srWindow.Left + 1 != width ||
            csbi.srWindow.Bottom - csbi.srWindow.Top + 1 != height)
        {
            return;
        }

        if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            birdY -= 2;
        }
        else
        {
            birdY += 1;
        }

        if (birdY < 0)
            birdY = 0;
        if (birdY >= height)
            birdY = height - 0.2;

        drawPipes(offset, true);
        offset++;
        if (offset == 14)
        {
            offset = 0;
            for (int i = 0; i < 19; i++)
            {
                top[i] = top[i + 1];
                bottom[i] = bottom[i + 1];
            }
            generatePipes(height, 19);
        }
        drawPipes(offset);
        put(wbird.c_str(), birdX, birdY);

        bool hit = false;
        for (int k = 0; k < (int)bird.length(); k++)
        {
            int px = birdX + k + offset;
            int i = px / 14;
            if (px % 14 == 0 && i < 20 && (birdY < top[i] || birdY >= bottom[i]))
                hit = true;
        }
        if (hit)
        {
            const wchar_t *msg = L" GAME OVER - press ESC ";
            put(msg, (width - (int)wcslen(msg)) / 2, 1);
            while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000))
                Sleep(50);
            return;
        }

        Sleep(speed);

        for (int k = 0; k < (int)bird.length(); k++)
        {
            int x = birdX + k;
            int px = x + offset;
            int i = px / 14;
            bool isPipe = (px % 14 == 0) && i < 20 && (birdY < top[i] || birdY >= bottom[i]);
            put(isPipe ? L"\u2588" : L" ", x, birdY);
        }
    }
}

bool FlappyBird::generatePipes(int height, int from)
{
    const int gap = 8;
    const int firstSlot = 6;

    if (from == 0)
    {
        for (int i = 0; i < 20; i++)
        {
            top[i] = 0;
            bottom[i] = height;
        }
        reach.assign(height, 1);
        from = firstSlot;
    }

    for (int i = from; i < 20; i++)
    {
        vector<int> ok;
        vector<char> tmp;
        for (int c = 1; c <= height - gap - 2; c++)
            if (passGap(reach, c, gap, height, (int)bird.length(), tmp))
                ok.push_back(c);

        int t;
        if (!ok.empty())
        {
            t = ok[rand() % ok.size()];
            passGap(reach, t, gap, height, (int)bird.length(), tmp);
            reach = tmp;
        }
        else
        {
            t = 1 + rand() % (height - gap - 2);
            reach.assign(height, 1);
        }
        top[i] = t;
        bottom[i] = t + gap;
    }
    return true;
}

bool FlappyBird::drawPipes(int offset, bool erase)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    const wchar_t *g = erase ? L" " : L"\u2588";

    for (int i = 0; i < 20; i++)
    {
        int x = i * 14 - offset;
        if (x < 0)
            continue;

        for (int j = 0; j < top[i]; j++)
            put(g, x, j);

        for (int j = bottom[i]; j < height; j++)
            put(g, x, j);
    }
    return true;
}

int FlappyBird::start(int speed)
{
    auto start = std::chrono::steady_clock::now();

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int startWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int startHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int width = startWidth;
    int height = startHeight;
    generatePipes(height);

    while (true)
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
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
            cout << "You survived for " << duration << " seconds!" << endl;
            return duration;
        }
        else
        {
            if (drawPipes())
            {
                fly(speed);
            }
            else
            {
                cout << "" << endl;
                auto end = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
                cout << "You survived for " << duration << " seconds!" << endl;
                return duration;
            }
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout << "You survived for " << duration << " seconds!" << endl;
    return duration;
}

FlappyBird::~FlappyBird()
{
    SetStdHandle(STD_OUTPUT_HANDLE, hOriginal);
    SetConsoleActiveScreenBuffer(hOriginal);
    CloseHandle(hGame);
    cout << "Game END" << endl;
}

FlappyBird::FlappyBird()
{
    cout << "Flappy Bird is ready to fly! DO NOT RESIZE CONSOLE!" << endl;
    hOriginal = GetStdHandle(STD_OUTPUT_HANDLE);
    hGame = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
    SetConsoleActiveScreenBuffer(hGame);
    SetStdHandle(STD_OUTPUT_HANDLE, hGame);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD bufferSize = {80, 25};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SMALL_RECT window = {0, 0, 79, 24};
    SetConsoleWindowInfo(hConsole, TRUE, &window);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    clearConsole();
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    bird = R"bird(/^v^\)bird";

    birdX = (width - (int)bird.length()) / 2;
    birdY = height / 2;

    COORD pos;
    pos.X = birdX;
    pos.Y = birdY;

    SetConsoleCursorPosition(hConsole, pos);
}

void FlappyBird::clearConsole()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD count;

    COORD home = {0, 0};

    FillConsoleOutputCharacter(
        hConsole, ' ', cellCount, home, &count);

    FillConsoleOutputAttribute(
        hConsole, csbi.wAttributes, cellCount, home, &count);

    SetConsoleCursorPosition(hConsole, home);
}