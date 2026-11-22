#include "Tetris.h"
#include <iostream>
#include <string>
#include <random>
using namespace std;

Tetromino::Tetromino()
{
    char options[] = {'-', 'o', 'T', 'Z', '4'};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);
    char selectedOption = options[dis(gen)];

    switch (selectedOption)
    {
    case '-':
        shape[0][0] = "████";
        shape[1][0] = "    ";
        shape[2][0] = "    ";
        shape[3][0] = "    ";
        break;
    case 'o':
        shape[0][0] = "██";
        shape[1][0] = "██";
        shape[2][0] = "  ";
        shape[3][0] = "  ";
        break;
    case 'T':
        shape[0][0] = "███";
        shape[1][0] = " █ ";
        shape[2][0] = "   ";
        shape[3][0] = "   ";
        break;
    case 'Z':
        shape[0][0] = "██ ";
        shape[1][0] = " ██";
        shape[2][0] = "   ";
        shape[3][0] = "   ";
        break;
    case '4':
        shape[0][0] = "█  ";
        shape[1][0] = "██ ";
        shape[2][0] = " █ ";
        shape[3][0] = "   ";
        break;
    }
}