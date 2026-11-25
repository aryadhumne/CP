#include "scramble.h"
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
using namespace std;

scramble::scramble()
{
    score = 0;
    word = rand() % 30;
    lives = 3;
    scarmbled = words[word];
    selected = scarmbled;
    random_device rd;
    mt19937 g(rd());
    shuffle(scarmbled.begin(), scarmbled.end(), g);
}

scramble::~scramble() {}

int scramble::play()
{
    for (int i = 0; i < scarmbled.length(); i++)
    {
        cout << scarmbled[i] << " ";
    }
    cout << "\n You lives " << lives;
    cout << "\n Unscramble the word: ";

    cin >> guessed;
    while ((guessed != selected) && (lives != 0))
    {
        cout << "\n Wrong guess! Remaining lives " << --lives;
        cout << "\n Try Again! Unscramble the word: ";
        cin >> guessed;
        if (lives == 0)
        {
            cout << "\n You lost!!";
            return score;
        }
    }
    cout << "\n You Won!!";
    score++;
    return score;
}