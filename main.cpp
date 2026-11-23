#include <iostream>
#include <string>
#include "Flappy_Bird.h"
#include "Tetris.h"
#include "r_p_s.h"
#include "scramble.h"
#include "cardGuessing.h"
#include "hangman.h"
#include "tic_tac_toe.h"
#include "Auth.h"
using namespace std;

string currUser, currPass;

int main()
{
    Auth a;
    int game, score;
    if (!a.auth(currPass, currUser))
        return 0;

    do
    {
        cout << "\n********************* ULTIMATE GAME CONSOLE *********************\n";
        cout << " 1. Tic Tac Toe\n";
        cout << " 2. Rock Paper Scissors\n";
        cout << " 3. Hangman\n";
        cout << " 4. Card Guessing Game\n";
        cout << " 5. Word Scramble\n";
        cout << " 6. Tetris\n";
        cout << " 7. Flappy Bird\n";
        cout << " 0. EXIT\n";
        cout << " Input : ";
        cin >> game;

        switch (game)
        {
        case 1:
        {
            int mode;
            cout << "\n Entered TIC TAC TOE!!\n Enter mode Multiplayer(1) or Bot(2) : ";
            cin >> mode;
            tic_tac_toe P;
            if (mode == 1)
                P.play('u'); // compile time Polymorphism
            else
                P.play();
            break;
        }
        case 2:
        {
            cout << "\n Entered ROCK PAPER SCISSORS!!";
            r_p_s P;
            score = P.play();
            break;
        }
        case 3:
        {
            cout << "\n Entered HANGMAN!!";
            hangman P;
            score = P.play();
            break;
        }
        case 4:
        {
            cout << "\n Entered CARD GUESSING GAME!!";
            cardGuessing P;
            score = P.play();
            break;
        }
        case 5:
        {
            cout << "\n Entered WORD SCRAMBLE!!";
            scramble P;
            score = P.play();
            break;
        }
        case 6:
        {
            cout << "\n Entered TETRIS!";
            Tetris tetris;
            break;
        }
        case 7:
        {
            cout << "\n Entered FLAPPY BIRD!";
            FlappyBird bird;
            score = bird.start(85);
            if (score > -1)
                a.updateUserData(currUser, to_string(score));
            break;
        }
        case 0:
        {
            cout << "\n Goodbye!\n";
            break;
        }
        default:
        {
            cout << "\n INVALID INPUT!! Enter again!";
            break;
        }
        }
    } while (game != 0);

    return 0;
}
