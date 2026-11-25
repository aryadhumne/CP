#include "hangman.h"
#include <string>
#include <iostream>
using namespace std;

hangman::hangman()
{
    index = rand() % 15;
    score = 0;
}

hangman::~hangman() {}

bool checkifcanplay(string result)
{
    int flag = 0;
    for (int i = 0; i < result.length(); i++)
    {
        if (result[i] == '_')
        {
            flag = 1;
            return true;
        }
    }
    if (flag == 0)
    {
        return false;
    }
    return false;
}

int hangman::play()
{
    cout << logo;
    string result = "";
    int len = words[index].length();
    string current = words[index];
    cout << "\n\n";
    for (int i = 0; i < len; i++)
    {
        cout << "_ ";
        result.append("_");
    }
    cout << "\n You have " << lives;
    while ((lives != 0) && (checkifcanplay(result)))
    {
        int flag = 0;
        cout << "\n Enter character guess in the word: ";
        cin >> input;
        for (int i = 0; i < len; i++)
        {
            if (input == current[i])
            {

                result[i] = current[i];
                flag = 1;
            }
        }
        if (flag == 0)
        {
            --lives;
            cout << man[lives] << endl;
        }
        cout << result;
    }
    if (lives == 0)
    {
        cout << "\n GAME END!";
        cout << words[index];
    }
    if (!checkifcanplay(result))
    {
        cout << "\n You've guessed it right!!";
        score++;
    }
    return score;
}
