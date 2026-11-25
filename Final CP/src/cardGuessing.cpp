#include "cardGuessing.h"
#include <string>
#include <iostream>
using namespace std;

cardGuessing::cardGuessing()
{
    card = rand() % 13;
    card++;
    sym = rand() % 4;
    setSymbol(sym);
    lives = 7;
    score = 0;
}

cardGuessing::~cardGuessing() {}

int cardGuessing::play()
{
    cout << "\n You have " << lives << " lives.";
    do
    {

        cout << "\n Guess the random drawn card between 1(Ace) to 13(King): ";
        cin >> guess;
        cout << "\n Enter guess symbol \n S(spade)\n H(heart)\n C(club)\nD(Diamond): ";
        cin >> guessSym;

    } while ((!checkMatch(guessSym, guess)) && (lives != 0));
    return score;
}

bool cardGuessing::checkMatch(char guessSym, int guess)
{
    if ((guessSym == cardSym) && (guess == card))
    {
        switch (guess)
        {
        case 1:
            cout << "\n You guessed it right! It is an ACE!! of.	";
            break;
        case 11:
            cout << "\n You guessed it right! It is a JACK!! of.	";
            break;
        case 12:
            cout << "\n You guessed it right! It is a QUEEN!! of.	";
            break;
        case 13:
            cout << "\n	You	guessed	it	right!	It	is	a KING!! of.	";
            break;
        default:
            cout << "\n	You	guessed	it	right!	It	is	" << guess + 1 << " of.";
            break;
        }

        cout << symbols[sym];
        score++;
        return true;
    }
    else
    {
        guess--;
        if (guess > card)
        {
            cout << "\n Guess lower.";
        }
        else if (guess < card)
        {
            cout << "\n Guess higher.";
        }
        if (guessSym != cardSym)
        {
            cout << "\n Wrong suit !!";
        }
        else
        {
            cout << "\n You guessed the right suit!! Try for the number now !";
        }
        cout << "\n " << --lives << " lives left.";
        return false;
    }
}

void cardGuessing::setSymbol(int sym)
{
    if (sym == 0)
    {
        cardSym = 'S';
    }
    else if (sym == 1)
    {
        cardSym = 'H';
    }
    else if (sym == 2)
    {
        cardSym = 'D';
    }
    else
    {
        cardSym = 'C';
    }
}
