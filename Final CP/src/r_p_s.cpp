#include <iostream>
#include <cstdlib>
#include <string>
#include "r_p_s.h"
using namespace std;

Rock::Rock()
{
    rock = R"(   
    _______
---'   ____)
      (_____)
      (_____)
      (____)
---.__(___)
)";
}

Paper::Paper()
{
    paper = R"(  
    _______
---'   ____)____
          ______)
          _______)
         _______)
---.__________)
)";
}

Scissor::Scissor()
{
    scissor = R"(
    _______
---'   ____)____
          ______)
       __________)
      (____)
---.__(___)
)";
}

r_p_s::r_p_s()
{
    score = 0;
} // constructor empty default

r_p_s::~r_p_s() {} // destructor empty pure

int r_p_s::play()
{
    Play = 'Y';
    while (Play == 'Y')
    {
        int call;
        cout << "\n Enter 0 for rock, 1 for paper, 2 for scissor: ";
        cin >> call;
        switch (call)
        {
        case 0:
            cout << rock << endl;
            break;
        case 1:
            cout << paper << endl;
            break;
        case 2:
            cout << scissor << endl;
            break;
        default:
            cout << "\n Wrong Input!!";
            break;
        }
        int compCall = rand() % 3;
        switch (compCall)

        {
        case 0:
            cout << rock << endl;
            break;
        case 1:
            cout << paper << endl;
            break;
        case 2:
            cout << scissor << endl;
            break;
        }

        if ((call == 0) && (compCall == 2))
        {
            score++;
            cout << "\n You won!! GREAT!!";
        }
        else if ((call == 1) && (compCall == 0))
        {
            score++;
            cout << "\n You won!! GREAT!!";
        }
        else if ((call == 2) && (compCall == 1))
        {
            score++;
            cout << "\n You won!! GREAT!!";
        }
        else if (call == compCall)
        {
            cout << "\n It's a tie! No points!";
        }
        else
        {
            cout << "\n OOPs! you lost!";
        }
        cout << "\n Do you want to play again? (Y or N): ";
        cin >> Play;
    }
    return score;
}
