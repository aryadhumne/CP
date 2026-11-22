#include <iostream>
#include "Flappy_Bird.h"
#include <string>
#include "Tetris.h"
#include <fstream>
using namespace std;
static const string AUTH_FILE = "auth.txt";
string currUser, currPass;

static bool parseLine(const string &line, string &user, string &pass, string &data)
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

static bool validField(const string &s, bool allowEmpty = false)
{
    if (s.empty())
        return allowEmpty;
    return s.find_first_of("|\r\n") == string::npos;
}

static vector<string> readAllLines()
{
    vector<string> lines;
    ifstream in(AUTH_FILE);
    string line;
    while (getline(in, line))
        if (!line.empty())
            lines.push_back(line);
    return lines;
}

static bool writeAllLines(const vector<string> &lines)
{
    ofstream out(AUTH_FILE, ios::trunc);
    if (!out)
        return false;
    for (const string &l : lines)
        out << l << '\n';
    return true;
}

bool userExists(const string &user)
{
    for (const string &line : readAllLines())
    {
        string u, p, d;
        if (parseLine(line, u, p, d) && u == user)
            return true;
    }
    return false;
}

bool appendUser(const string &user, const string &pass, const string &data = "")
{
    if (!validField(user) || !validField(pass) || !validField(data, true))
        return false;
    ofstream out(AUTH_FILE, ios::app);
    if (!out)
        return false;
    out << user << '|' << pass << '|' << data << '\n';
    return true;
}

bool updateUserData(const string &user, const string &data)
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

bool registerUser(const string &user, const string &pass, const string &data = "")
{
    if (userExists(user))
        return false;
    return appendUser(user, pass, data);
}

bool checkLogin(const string &user, const string &pass)
{
    for (const string &line : readAllLines())
    {
        string u, p, d;
        if (parseLine(line, u, p, d) && u == user)
            return p == pass;
    }
    return false;
}

bool readUserData(const string &user, string &data)
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

bool clearUserData(const string &user)
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

bool removeUser(const string &user)
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

bool auth()
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

int main()
{
    if (!auth())
        return 0;

    Tetris tetris;

    /*FlappyBird bird;
    int score = bird.start(85);
    if (score > -1)
    {
        updateUserData(currUser, to_string(score));
    }*/
    return 0;
}

/*#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
sf::RenderWindow window(sf::VideoMode({800, 600}), "My Mini Games");
    window.setFramerateLimit(60);
sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        return 1;

    int game = 0;
    vector<sf::Vector2i> snake = {{10, 10}, {9, 10}, {8, 10}};
    sf::Vector2i direction = {1, 0};
    sf::Vector2i food = {15, 8};
    int score = 0;
    bool snakeOver = false;
    float timer = 0;
    sf::Clock clock;
    char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    char player = 'X';
    bool ticOver = false;
    char winner = ' ';
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        while (const optional event = window.pollEvent()) {
     if (event->is<sf::Event::Closed>())
                window.close();
if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    game = 0;
                }
if (game == 0) {
                    if (key->code == sf::Keyboard::Key::Num1)
                        game = 1;
                    if (key->code == sf::Keyboard::Key::Num2)
                        game = 2;
                }
 if (game == 1) {
                    if (key->code == sf::Keyboard::Key::W && direction.y != 1)
                        direction = {0, -1};
                    if (key->code == sf::Keyboard::Key::S && direction.y != -1)
                        direction = {0, 1};
                    if (key->code == sf::Keyboard::Key::A && direction.x != 1)
                        direction = {-1, 0};
                    if (key->code == sf::Keyboard::Key::D && direction.x != -1)
                        direction = {1, 0};
                    if (key->code == sf::Keyboard::Key::R && snakeOver) {
                        snake = {{10, 10}, {9, 10}, {8, 10}};
                        direction = {1, 0};
                        food = {15, 8};
                        score = 0;
                        snakeOver = false;
                        timer = 0;
                    }
                }
 if (game == 2 && key->code == sf::Keyboard::Key::R) {
                    for (int i = 0; i < 9; i++)
                        board[i] = ' ';
                    player = 'X';
                    winner = ' ';
                    ticOver = false;
                }
            }
if (game == 2) {
                if (const auto* mouse =
                    event->getIf<sf::Event::MouseButtonPressed>()) {
if (mouse->button == sf::Mouse::Button::Left && !ticOver) {
                        int col = mouse->position.x / 200;
                        int row = (mouse->position.y - 100) / 130;
                        if (mouse->position.x >= 0 &&
                            mouse->position.x < 600 &&
                            mouse->position.y >= 100 &&
                            mouse->position.y < 490 &&
                            col < 3 && row < 3) {

                            int place = row * 3 + col;

                            if (board[place] == ' ') {
                                board[place] = player;

                                // Check all possible winning lines
                                if ((board[0] == player && board[1] == player && board[2] == player) ||
                                    (board[3] == player && board[4] == player && board[5] == player) ||
                                    (board[6] == player && board[7] == player && board[8] == player) ||
                                    (board[0] == player && board[3] == player && board[6] == player) ||
                                    (board[1] == player && board[4] == player && board[7] == player) ||
                                    (board[2] == player && board[5] == player && board[8] == player) ||
                                    (board[0] == player && board[4] == player && board[8] == player) ||
                                    (board[2] == player && board[4] == player && board[6] == player)) {
                                    winner = player;
                                    ticOver = true;
                                } else {
                                    bool full = true;
                                    for (int i = 0; i < 9; i++) {
                                        if (board[i] == ' ')
                                            full = false;
                                    }

                                    if (full)
                                        ticOver = true;
                                    else if (player == 'X')
                                        player = 'O';
                                    else
                                        player = 'X';
                                }
                            }
                        }
                    }
                }
            }
        }

        // Move Snake every few milliseconds
        if (game == 1 && !snakeOver) {
            timer += dt;

            if (timer > 0.15f) {
                timer = 0;

                sf::Vector2i head = snake[0];
                head.x += direction.x;
                head.y += direction.y;

                if (head.x < 0 || head.x >= 20 ||
                    head.y < 0 || head.y >= 20) {
                    snakeOver = true;
                } else {
                    bool eating = (head == food);

                    for (int i = 0; i < (int)snake.size() - (eating ? 0 : 1); i++) {
                        if (snake[i] == head)
                            snakeOver = true;
                    }

                    if (!snakeOver) {
                        snake.insert(snake.begin(), head);

                        if (eating) {
                            score += 10;
                            food = {rand() % 20, rand() % 20};
                        } else {
                            snake.pop_back();
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(25, 30, 45));

        // Draw the menu
        if (game == 0) {
            sf::Text title(font, "MY MINI GAMES", 42);
            title.setFillColor(sf::Color::Yellow);
            title.setPosition({235, 120});
            window.draw(title);

            sf::Text options(font, "Press 1 - Snake\n\nPress 2 - Tic Tac Toe\n\nPress ESC - Quit", 28);
            options.setPosition({250, 240});
            window.draw(options);
        }

        // Draw Snake
        else if (game == 1) {
            sf::RectangleShape square({28, 28});

            square.setFillColor(sf::Color::Red);
            square.setPosition({food.x * 30.f, food.y * 30.f});
            window.draw(square);

            square.setFillColor(sf::Color::Green);
            for (auto part : snake) {
                square.setPosition({part.x * 30.f, part.y * 30.f});
                window.draw(square);
            }

            sf::Text points(font, "Score: " + to_string(score), 22);
            points.setPosition({620, 30});
            window.draw(points);

            sf::Text help(font, "W A S D to move\nESC: Menu", 18);
            help.setPosition({620, 80});
            window.draw(help);

            if (snakeOver) {
                sf::Text over(font, "GAME OVER\nPress R to restart", 30);
                over.setFillColor(sf::Color::Yellow);
                over.setPosition({260, 250});
                window.draw(over);
            }
        }

        // Draw Tic Tac Toe
        else if (game == 2) {
            sf::Text title(font, "TIC TAC TOE", 32);
            title.setPosition({280, 30});
            window.draw(title);

            for (int i = 1; i < 3; i++) {
                sf::RectangleShape line({4, 390});
                line.setPosition({i * 200.f, 100});
                window.draw(line);

                sf::RectangleShape line2({600, 4});
                line2.setPosition({0, 100 + i * 130.f});
                window.draw(line2);
            }

            for (int i = 0; i < 9; i++) {
                if (board[i] != ' ') {
                    sf::Text mark(font, string(1, board[i]), 70);
                    mark.setFillColor(board[i] == 'X' ?
                        sf::Color::Cyan : sf::Color::Yellow);
                    mark.setPosition({i % 3 * 200.f + 70,
                                      100 + i / 3 * 130.f + 20});
                    window.draw(mark);
                }
            }

            sf::Text help(font, "Click a square to play. ESC: Menu", 20);
            help.setPosition({180, 520});
            window.draw(help);

            if (ticOver) {
                string message = (winner == ' ') ? "It's a draw!" :
                    string("Player ") + winner + " wins!";

                sf::Text result(font, message + "  Press R to restart", 24);
                result.setFillColor(sf::Color::Yellow);
                result.setPosition({170, 560});
                window.draw(result);
            } else {
                sf::Text turn(font, string("Turn: Player ") + player, 22);
                turn.setPosition({320, 490});
                window.draw(turn);
            }
        }

        window.display();
    }

    return 0;
}
*/