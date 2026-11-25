#include "tic_tac_toe.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Auth.h"
using namespace std;

tic_tac_toe::tic_tac_toe()
{ // constructor default
    score = 0;
    cout << "\n Enter if you want X or O: ";
    cin >> user;
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            board[j][i] = '.';
        }
    }
    if (user == 'O')
    {
        comp = 'X';
        user2 = comp;
    }
    else
    {
        comp = 'O';

        user2 = comp;
    }
}

tic_tac_toe::~tic_tac_toe()
{
    cout << "\n Tic Tac Toe ended!";
}

void tic_tac_toe::display()
{
    int k = 1;
    cout << "\n\n___________\n";
    for (int j = 0; j < 3; j++)
    {
        cout << '|';
        for (int i = 0; i < 3; i++)
        {
            cout.width(2);
            cout << board[j][i];
            if ((user == board[j][i]) || (user == board[j][i]))
            {
                cout.width(2);
            }
            cout.width(1);
            cout << '|';
            k++;
        }
        cout << "\n___________\n";
    }
}

bool tic_tac_toe::clearBoard()
{
    int flag = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == '.')
            {
                flag = 1;
                break;
            }
        }
    }
    if (flag == 1)
        return true;
    else
        return false;
}

bool tic_tac_toe::initialize(int pos, char symbol)
{
    int flag = 0;
    if (pos < 10)
    {
        switch (pos)
        {
        case 1:
            if (board[0][0] == '.')
            {
                board[0][0] = symbol;
                flag = 1;
            }
            break;
        case 2:
            if (board[0][1] == '.')
            {
                board[0][1] = symbol;
                flag = 1;
            }
            break;
        case 3:
            if (board[0][2] == '.')
            {
                board[0][2] = symbol;
                flag = 1;
            }
            break;
        case 4:
            if (board[1][0] == '.')
            {
                board[1][0] = symbol;
                flag = 1;
            }
            break;
        case 5:
            if (board[1][1] == '.')
            {
                board[1][1] = symbol;
                flag = 1;
            }
            break;
        case 6:
            if (board[1][2] == '.')
            {
                board[1][2] = symbol;
                flag = 1;
            }
            break;
        case 7:
            if (board[2][0] == '.')
            {
                board[2][0] = symbol;
                flag = 1;
            }
            break;
        case 8:
            if (board[2][1] == '.')
            {
                board[2][1] = symbol;
                flag = 1;
            }
            break;
        case 9:
            if (board[2][2] == '.')
            {
                board[2][2] = symbol;
                flag = 1;
            }
            break;
        }
        return true;
    }
    else
    {
        cout << "\n Error!";
        return false;
    }
}

bool tic_tac_toe::checkIfWin(char symbol)
{
    int flag = 0;

    // ROWS
    for (int i = 0; i < 3; i++)
    {
        if ((board[i][0] == board[i][1]) && (board[i][1] == board[i][2]) && (board[i][0] == symbol))
        {
            flag = 1;
            break;
        }
    }
    // COLUMNS
    for (int j = 0; j < 3; j++)
    {
        if ((board[0][j] == board[1][j]) && (board[1][j] == board[2][j]) && (board[0][j] == symbol))
        {
            flag = 1;
            break;
        }
    }
    // DIAGONALS
    if ((board[0][0] == board[1][1]) && (board[1][1] == board[2][2]) && (board[0][0] == symbol))
    {
        flag = 1;
    }
    if ((board[2][0] == board[1][1]) && (board[1][1] == board[0][2]) && (board[2][0] == symbol))
    {
        flag = 1;
    }

    // CHECK
    if (flag == 0)
    {
        return false;
    }
    else if (flag == 1)
    {
        return true;
    }
    return false;
}

void tic_tac_toe::compplay()
{
    int compCall;
    do
    {
        compCall = rand() % 10;
    } while ((!initialize(compCall, comp)) && (compCall != 0));
}

int tic_tac_toe::play(void)
{
    int pos;
    display();
    do
    {
        cout << "\n Enter position(1 to 9): ";
        cin >> pos;
        initialize(pos, user);
        display();
        compplay();
        if ((!checkIfWin(user)) && (clearBoard()))
        {
            display();
            compplay();
            display();
        }
        else if (pos > 10)
        {
            cout << "\n wrong position entered! Game End!";
            exit(1);
        }
    } while ((!checkIfWin(user)) && (!checkIfWin(comp)) && (clearBoard()));
    if (checkIfWin(user))
    {
        score++;
        cout << "\n Congratulations you won!!";
        return score;
    }
    else if (checkIfWin(comp))
    {
        cout << "\n OOPS! you lost!";
    }
    return score;
}

int tic_tac_toe::play(char u)
{
    string username, password;
    Auth a;
    int pos, score2;
    bool flag = a.auth(password, username);
    if (flag == 0)
    {
        cout << "\n Invalid credentials!";
        exit(1);
    }
    display();
    do
    {
        cout << "\n " << username << " Enter position(1 to 9): ";
        cin >> pos;
        initialize(pos, user);
        if ((!checkIfWin(user)) && (clearBoard()))
        {
            display();
            cout << "\n " << username << " Enter position(1 to 9): ";
            cin >> pos;
            initialize(pos, user2);
            display();
        }
        else if (pos > 10)
        {
            cout << "\n wrong position entered! Game End!";
            exit(1);
        }
    } while ((!checkIfWin(user)) && (!checkIfWin(user2)) && (clearBoard()));
    if (checkIfWin(user))
    {
        score++;
        cout << "\n Congratulations Player 1 you won!!\n Score: " << score;
        return score;
    }
    else if (checkIfWin(user2))
    {
        score2++;
        cout << "\n Congratulations " << username << " you won!!\n Score: " << score2;
    }
    return score;
}
