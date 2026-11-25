#include <iostream>
#include <string>
#include "Auth.h"
#include "tic_tac_toe.h"
#include "r_p_s.h"
#include "hangman.h"
#include "cardGuessing.h"
#include "scramble.h"
#include "Tetris.h"
#include "Flappy_Bird.h"
#include "snake.h"
#include "pong.h"
#include "Breakout.h"
#include "DoodleJump.h"
#include "SpaceInvaders.h"
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
        cout << " 8. Snake\n";
        cout << " 9. Pong\n";
        cout << " 10. Breakout\n";
        cout << " 11. Doodle Jump\n";
        cout << " 12. Space Invaders\n";
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
            {
                score = P.play('u'); // compile time Polymorphism
                string data = "Tic Tac Toe : " + to_string(score);
                if (score > -1)
                    a.updateUserData(currUser, data);
            }
            else
            {
                score = P.play();
                string data = "Tic Tac Toe : " + to_string(score);
                if (score > -1)
                    a.updateUserData(currUser, data);
            }
            break;
        }
        case 2:
        {
            cout << "\n Entered ROCK PAPER SCISSORS!!";
            r_p_s P;
            score = P.play();
            string data = "Rock Paper Scissors : " + to_string(score);
            if (score > -1)
                a.updateUserData(currUser, data);
            break;
        }
        case 3:
        {
            cout << "\n Entered HANGMAN!!";
            hangman P;
            score = P.play();
            string data = "Hangman : " + to_string(score);
            if (score > -1)
                a.updateUserData(currUser, data);
            break;
        }
        case 4:
        {
            cout << "\n Entered CARD GUESSING GAME!!";
            cardGuessing P;
            score = P.play();
            string data = "Card Guessing : " + to_string(score);
            if (score > -1)
                a.updateUserData(currUser, data);
            break;
        }
        case 5:
        {
            cout << "\n Entered WORD SCRAMBLE!!";
            scramble P;
            score = P.play();
            string data = "Scramble : " + to_string(score);
            if (score > -1)
                a.updateUserData(currUser, data);
            break;
        }
        case 6:
        {
            cout << "\n Entered TETRIS!";
            Tetris tetris;
            score = tetris.start();
            string data = "Tetris : " + to_string(score);
            if (score > -1)
                a.updateUserData(currUser, data);
            break;
        }
        case 7:
        {
            cout << "\n Entered FLAPPY BIRD!";
            FlappyBird bird;
            score = bird.start(85);
            string data = "Flappy Bird : " + to_string(score);
            if (score > -1)
                a.updateUserData(currUser, data);
            break;
        }
        case 8:
        {
            cout << "\n Entered SNAKE!";
            Snake snake;
            score = snake.play();
            string data = "Snake : " + to_string(score);
            if (score > -1)
                a.updateUserData(currUser, data);
            break;
        }
        case 9:
        {
            cout << "\n Entered PONG!";
            Pong pong;
            score = pong.play();
            string data = "Pong : " + to_string(score);
            if (score > -1)
                a.updateUserData(currUser, data);
            break;
        }
        case 10:
{
    cout << "\n Entered BREAKOUT!";
    Breakout breakout;
    score = breakout.play();

    string data = "Breakout : " + to_string(score);

    if (score > -1)
        a.updateUserData(currUser, data);

    break;
}
case 11:
{
    cout << "\n Entered DOODLE JUMP!";
    DoodleJump doodle;
    score = doodle.play();

    string data = "Doodle Jump : " + to_string(score);

    if (score > -1)
        a.updateUserData(currUser, data);

    break;
}
case 12:
{
    cout << "\n Entered SPACE INVADERS!";
    SpaceInvaders space;
    score = space.play();

    string data = "Space Invaders : " + to_string(score);

    if (score > -1)
        a.updateUserData(currUser, data);

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
