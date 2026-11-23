#include <string>
#include <iostream>
#include <fstream>
#include "Auth.h"
#include <vector>
using namespace std;

bool Auth::parseLine(const string &line, string &user, string &pass, string &data)
{
    size_t p1 = line.find('|');
    if (p1 == string::npos)
        return false;
    size_t p2 = line.find('|', p1 + 1);
    if (p2 == string::npos)
        return false;
    user = line.substr(0, p1);
    pass = line.substr(p1 + 1, p2 - p1 - 1);
    data = line.substr(p2 + 1);
    return true;
}

bool Auth::validField(const string &s, bool allowEmpty)
{
    if (s.empty())
        return allowEmpty;
    return s.find_first_of("|\r\n") == string::npos;
}

vector<string> Auth::readAllLines()
{
    vector<string> lines;
    ifstream in(AUTH_FILE);
    string line;
    while (getline(in, line))
        if (!line.empty())
            lines.push_back(line);
    return lines;
}

bool Auth::writeAllLines(const vector<string> &lines)
{
    ofstream out(AUTH_FILE, ios::trunc);
    if (!out)
        return false;
    for (const string &l : lines)
        out << l << '\n';
    return true;
}

bool Auth::userExists(const string &user)
{
    for (const string &line : readAllLines())
    {
        string u, p, d;
        if (parseLine(line, u, p, d) && u == user)
            return true;
    }
    return false;
}

bool Auth::appendUser(const string &user, const string &pass, const string &data)
{
    if (!validField(user) || !validField(pass) || !validField(data, true))
        return false;
    ofstream out(AUTH_FILE, ios::app);
    if (!out)
        return false;
    out << user << '|' << pass << '|' << data << '\n';
    return true;
}

bool Auth::updateUserData(const string &user, const string &data)
{
    if (!validField(data, true))
        return false;

    vector<string> lines = readAllLines();
    bool found = false;
    for (string &line : lines)
    {
        string u, p, d;
        if (parseLine(line, u, p, d) && u == user)
        {
            line = u + '|' + p + '|' + data;
            found = true;
            break;
        }
    }
    return found && writeAllLines(lines);
}

bool Auth::registerUser(const string &user, const string &pass, const string &data)
{
    if (userExists(user))
        return false;
    return appendUser(user, pass, data);
}

bool Auth::checkLogin(const string &user, const string &pass)
{
    for (const string &line : readAllLines())
    {
        string u, p, d;
        if (parseLine(line, u, p, d) && u == user)
            return p == pass;
    }
    return false;
}

bool Auth::readUserData(const string &user, string &data)
{
    for (const string &line : readAllLines())
    {
        string u, p, d;
        if (parseLine(line, u, p, d) && u == user)
        {
            data = d;
            return true;
        }
    }
    return false;
}

bool Auth::clearUserData(const string &user)
{
    vector<string> lines = readAllLines();
    bool found = false;
    for (string &line : lines)
    {
        string u, p, d;
        if (parseLine(line, u, p, d) && u == user)
        {
            line = u + '|' + p + '|';
            found = true;
            break;
        }
    }
    return found && writeAllLines(lines);
}

bool Auth::removeUser(const string &user)
{
    vector<string> lines = readAllLines();
    vector<string> kept;
    bool found = false;
    for (const string &line : lines)
    {
        string u, p, d;
        if (parseLine(line, u, p, d) && u == user)
            found = true;
        else
            kept.push_back(line);
    }
    return found && writeAllLines(kept);
}

bool Auth::auth(string &currPass, string &currUser)
{
    int choice;
    ifstream file(AUTH_FILE);
    if (!file.is_open())
        return false;

    cout << "Enter a choice : \n1. Login\n2. Register\n3. Exit\n";
    cin >> choice;
    switch (choice)
    {
    case 1:
    {
        string user, pass;
        cout << "Enter username : ";
        cin >> user;
        cout << "Enter password : ";
        cin >> pass;
        if (checkLogin(user, pass))
        {
            cout << "Login successful\n";
            currPass = pass;
            currUser = user;
            return true;
        }
        else
        {
            cout << "Invalid username or password\n";
            return false;
        }
    }
    break;
    case 2:
    {
        string user, pass;
        cout << "Enter username : ";
        cin >> user;
        cout << "Enter password : ";
        cin >> pass;
        if (registerUser(user, pass))
        {
            cout << "Registration successful\n";
            currPass = pass;
            currUser = user;
            return true;
        }
        else
        {
            cout << "Username already exists\n";
            return false;
        }
    }
    break;
    default:
    {
        cout << "Exiting...\n";
        return false;
    }
    }

    file.close();
    return false;
}