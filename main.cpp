#include <SFML/Graphics.hpp>
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
