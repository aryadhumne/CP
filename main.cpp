
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <optional>

// ============================================================
// Shared settings and helper functions
// ============================================================

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 600;

const sf::Color BACKGROUND(18, 22, 38);
const sf::Color WHITE(240, 240, 245);
const sf::Color GREEN(70, 220, 120);
const sf::Color RED(245, 80, 90);
const sf::Color BLUE(80, 160, 255);
const sf::Color YELLOW(255, 210, 70);

sf::Font* gameFont = nullptr;

void drawText(sf::RenderWindow& window, const std::string& value,
              float x, float y, unsigned int size,
              sf::Color color = WHITE)
{
    if (!gameFont) return;

    sf::Text text(*gameFont, value, size);
    text.setFillColor(color);
    text.setPosition({x, y});
    window.draw(text);
}

bool overlaps(float ax, float ay, float aw, float ah,
              float bx, float by, float bw, float bh)
{
    return ax < bx + bw && ax + aw > bx &&
           ay < by + bh && ay + ah > by;
}

float clampValue(float value, float low, float high)
{
    return std::max(low, std::min(value, high));
}

// ============================================================
// OOP base class
// Every game inherits from this class.
// ============================================================

class Game
{
public:
    virtual ~Game() = default;

    virtual std::string name() const = 0;
    virtual void reset() = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

// ============================================================
// 1. Snake
// ============================================================

class SnakeGame : public Game
{
    std::vector<sf::Vector2i> body;
    sf::Vector2i direction{1, 0};
    sf::Vector2i food{10, 10};
    float timer = 0.f;
    bool over = false;
    int score = 0;
    static constexpr int CELL = 20;

public:
    std::string name() const override { return "Snake"; }

    void reset() override
    {
        body = {{10, 10}, {9, 10}, {8, 10}};
        direction = {1, 0};
        food = {18, 14};
        timer = 0.f;
        score = 0;
        over = false;
    }

    void turn(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Key::Up && direction.y != 1)
            direction = {0, -1};
        else if (key == sf::Keyboard::Key::Down && direction.y != -1)
            direction = {0, 1};
        else if (key == sf::Keyboard::Key::Left && direction.x != 1)
            direction = {-1, 0};
        else if (key == sf::Keyboard::Key::Right && direction.x != -1)
            direction = {1, 0};
    }

    void update(float dt) override
    {
        if (over) return;
        timer += dt;
        if (timer < 0.12f) return;
        timer = 0.f;

        sf::Vector2i head = body.front() + direction;

        if (head.x < 0 || head.x >= 40 ||
            head.y < 0 || head.y >= 30)
        {
            over = true;
            return;
        }

        bool eating = (head == food);
        std::size_t limit = body.size() - (eating ? 0 : 1);

        for (std::size_t i = 0; i < limit; ++i)
        {
            if (body[i] == head)
            {
                over = true;
                return;
            }
        }

        body.insert(body.begin(), head);

        if (eating)
        {
            ++score;
            food = {std::rand() % 38 + 1, std::rand() % 28 + 1};
        }
        else
        {
            body.pop_back();
        }
    }

    void draw(sf::RenderWindow& window) override
    {
        sf::RectangleShape block({18.f, 18.f});
        block.setFillColor(RED);
        block.setPosition({food.x * 20.f, food.y * 20.f});
        window.draw(block);

        block.setFillColor(GREEN);
        for (const auto& part : body)
        {
            block.setPosition({part.x * 20.f, part.y * 20.f});
            window.draw(block);
        }

        drawText(window, "SNAKE  |  Score: " + std::to_string(score),
                 10, 8, 20);
        if (over)
            drawText(window, "GAME OVER - Press R to restart",
                     180, 280, 26, YELLOW);
    }

    void handleKey(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Key::R && over) reset();
        else turn(key);
    }
};

// ============================================================
// 2. Pong
// ============================================================

class PongGame : public Game
{
    sf::RectangleShape left{{14.f, 100.f}};
    sf::RectangleShape right{{14.f, 100.f}};
    sf::CircleShape ball{10.f};
    sf::Vector2f velocity{260.f, 190.f};
    int leftScore = 0;
    int rightScore = 0;

public:
    std::string name() const override { return "Pong"; }

    void reset() override
    {
        left.setPosition({30.f, 250.f});
        right.setPosition({756.f, 250.f});
        ball.setPosition({395.f, 295.f});
        velocity = {260.f, 190.f};
        leftScore = rightScore = 0;
    }

    void update(float dt) override
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            left.move({0.f, -360.f * dt});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            left.move({0.f, 360.f * dt});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            right.move({0.f, -360.f * dt});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            right.move({0.f, 360.f * dt});

        left.setPosition({30.f, clampValue(left.getPosition().y, 0.f, 500.f)});
        right.setPosition({756.f, clampValue(right.getPosition().y, 0.f, 500.f)});

        ball.move(velocity * dt);
        sf::Vector2f p = ball.getPosition();

        if (p.y <= 0.f || p.y >= 580.f)
            velocity.y = -velocity.y;

        if (overlaps(p.x, p.y, 20, 20,
                     left.getPosition().x, left.getPosition().y, 14, 100) ||
            overlaps(p.x, p.y, 20, 20,
                     right.getPosition().x, right.getPosition().y, 14, 100))
            velocity.x = -velocity.x;

        if (p.x < 0.f || p.x > 800.f)
        {
            if (p.x < 0.f) ++rightScore;
            else ++leftScore;

            ball.setPosition({390.f, 290.f});
            velocity.x = (p.x < 0.f) ? 260.f : -260.f;
            velocity.y = 190.f;
        }
    }

    void draw(sf::RenderWindow& window) override
    {
        sf::RectangleShape line({3.f, 600.f});
        line.setPosition({398.f, 0.f});
        line.setFillColor(sf::Color(100, 100, 120));
        window.draw(line);

        left.setFillColor(GREEN);
        right.setFillColor(BLUE);
        ball.setFillColor(WHITE);
        window.draw(left);
        window.draw(right);
        window.draw(ball);

        drawText(window, std::to_string(leftScore) + "     " +
                 std::to_string(rightScore), 345, 20, 36, YELLOW);
        drawText(window, "W/S", 25, 560, 18);
        drawText(window, "UP/DOWN", 700, 560, 18);
    }
};

// ============================================================
// 3. Breakout
// ============================================================

class BreakoutGame : public Game
{
    sf::RectangleShape paddle{{110.f, 16.f}};
    sf::CircleShape ball{9.f};
    sf::Vector2f velocity{230.f, -260.f};
    std::vector<sf::RectangleShape> bricks;
    bool over = false;
    bool won = false;
    int score = 0;

public:
    std::string name() const override { return "Breakout"; }

    void reset() override
    {
        paddle.setPosition({345.f, 550.f});
        ball.setPosition({395.f, 510.f});
        velocity = {230.f, -260.f};
        bricks.clear();
        score = 0;
        over = false;
        won = false;

        sf::Color colors[] = {RED, YELLOW, GREEN, BLUE};

        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < 10; ++col)
            {
                sf::RectangleShape brick({72.f, 22.f});
                brick.setPosition({8.f + col * 79.f, 55.f + row * 31.f});
                brick.setFillColor(colors[row]);
                bricks.push_back(brick);
            }
        }
    }

    void update(float dt) override
    {
        if (over || won) return;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            paddle.move({-420.f * dt, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            paddle.move({420.f * dt, 0.f});

        paddle.setPosition({clampValue(paddle.getPosition().x, 0.f, 690.f),
                            550.f});

        ball.move(velocity * dt);
        sf::Vector2f p = ball.getPosition();

        if (p.x <= 0.f || p.x >= 782.f) velocity.x = -velocity.x;
        if (p.y <= 0.f) velocity.y = -velocity.y;

        if (overlaps(p.x, p.y, 18, 18,
                     paddle.getPosition().x, 550.f, 110.f, 16.f)
            && velocity.y > 0)
        {
            velocity.y = -std::abs(velocity.y);
            velocity.x = (p.x + 9.f - paddle.getPosition().x - 55.f) * 4.f;
        }

        for (auto it = bricks.begin(); it != bricks.end(); ++it)
        {
            sf::Vector2f b = it->getPosition();
            if (overlaps(p.x, p.y, 18, 18, b.x, b.y, 72, 22))
            {
                bricks.erase(it);
                velocity.y = -velocity.y;
                ++score;
                break;
            }
        }

        if (p.y > 600.f) over = true;
        if (bricks.empty()) won = true;
    }

    void draw(sf::RenderWindow& window) override
    {
        paddle.setFillColor(WHITE);
        ball.setFillColor(YELLOW);
        window.draw(paddle);
        window.draw(ball);

        for (auto& brick : bricks) window.draw(brick);

        drawText(window, "BREAKOUT  |  Score: " + std::to_string(score),
                 10, 10, 20);
        if (over) drawText(window, "BALL LOST - Press R to restart",
                           185, 300, 25, RED);
        if (won) drawText(window, "YOU WIN! - Press R to play again",
                          160, 300, 25, GREEN);
    }

    void handleKey(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Key::R && (over || won)) reset();
    }
};

// ============================================================
// 4. Space Invaders
// ============================================================

class SpaceInvadersGame : public Game
{
    sf::RectangleShape player{{48.f, 22.f}};
    sf::RectangleShape bullet{{5.f, 14.f}};
    std::vector<sf::RectangleShape> enemies;
    bool bulletActive = false;
    bool over = false;
    int score = 0;
    float enemyTimer = 0.f;

public:
    std::string name() const override { return "Space Invaders"; }

    void reset() override
    {
        player.setPosition({376.f, 540.f});
        bulletActive = false;
        over = false;
        score = 0;
        enemyTimer = 0.f;
        enemies.clear();

        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 8; ++col)
            {
                sf::RectangleShape enemy({34.f, 24.f});
                enemy.setPosition({80.f + col * 75.f, 60.f + row * 48.f});
                enemy.setFillColor(GREEN);
                enemies.push_back(enemy);
            }
    }

    void shoot()
    {
        if (!bulletActive && !over)
        {
            bullet.setPosition({player.getPosition().x + 21.f,
                                player.getPosition().y - 14.f});
            bulletActive = true;
        }
    }

    void update(float dt) override
    {
        if (over) return;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            player.move({-320.f * dt, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            player.move({320.f * dt, 0.f});

        player.setPosition({clampValue(player.getPosition().x, 0.f, 752.f),
                            540.f});

        if (bulletActive)
        {
            bullet.move({0.f, -460.f * dt});
            sf::Vector2f b = bullet.getPosition();

            if (b.y < -20.f) bulletActive = false;

            for (auto it = enemies.begin(); it != enemies.end(); ++it)
            {
                sf::Vector2f e = it->getPosition();
                if (overlaps(b.x, b.y, 5, 14, e.x, e.y, 34, 24))
                {
                    enemies.erase(it);
                    bulletActive = false;
                    ++score;
                    break;
                }
            }
        }

        enemyTimer += dt;
        if (enemyTimer > 0.55f)
        {
            enemyTimer = 0.f;
            for (auto& enemy : enemies)
            {
                enemy.move({0.f, 12.f});
                if (enemy.getPosition().y > 510.f) over = true;
            }
        }

        if (enemies.empty()) over = true;
    }

    void draw(sf::RenderWindow& window) override
    {
        player.setFillColor(BLUE);
        bullet.setFillColor(YELLOW);
        window.draw(player);
        if (bulletActive) window.draw(bullet);
        for (auto& enemy : enemies) window.draw(enemy);

        drawText(window, "SPACE INVADERS  |  Score: " +
                 std::to_string(score), 10, 10, 20);

        if (over)
        {
            std::string message = enemies.empty() ? "YOU WIN!" : "GAME OVER";
            drawText(window, message + " - Press R to restart",
                     170, 300, 25, YELLOW);
        }
    }

    void handleKey(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Key::Space) shoot();
        if (key == sf::Keyboard::Key::R && over) reset();
    }
};

// ============================================================
// 5. Flappy Bird
// ============================================================

class FlappyGame : public Game
{
    sf::CircleShape bird{14.f};
    sf::RectangleShape topPipe{{75.f, 250.f}};
    sf::RectangleShape bottomPipe{{75.f, 250.f}};
    float birdY = 280.f;
    float velocityY = 0.f;
    float pipeX = 800.f;
    float gapY = 280.f;
    bool over = false;
    int score = 0;
    bool counted = false;

public:
    std::string name() const override { return "Flappy Bird"; }

    void reset() override
    {
        birdY = 280.f;
        velocityY = 0.f;
        pipeX = 800.f;
        gapY = 280.f;
        score = 0;
        over = false;
        counted = false;
        bird.setPosition({120.f, birdY});
    }

    void flap()
    {
        if (!over) velocityY = -350.f;
    }

    void update(float dt) override
    {
        if (over) return;

        velocityY += 700.f * dt;
        birdY += velocityY * dt;
        bird.setPosition({120.f, birdY});

        pipeX -= 220.f * dt;
        if (pipeX < -75.f)
        {
            pipeX = 800.f;
            gapY = static_cast<float>(std::rand() % 250 + 170);
            counted = false;
        }

        float topHeight = gapY - 90.f;
        float bottomY = gapY + 90.f;

        topPipe.setSize({75.f, topHeight});
        topPipe.setPosition({pipeX, 0.f});
        bottomPipe.setSize({75.f, 600.f - bottomY});
        bottomPipe.setPosition({pipeX, bottomY});

        if (birdY < 0.f || birdY > 572.f ||
            overlaps(120, birdY, 28, 28, pipeX, 0, 75, topHeight) ||
            overlaps(120, birdY, 28, 28, pipeX, bottomY, 75, 600 - bottomY))
        {
            over = true;
        }

        if (!counted && pipeX + 75.f < 120.f)
        {
            ++score;
            counted = true;
        }
    }

    void draw(sf::RenderWindow& window) override
    {
        bird.setFillColor(YELLOW);
        topPipe.setFillColor(GREEN);
        bottomPipe.setFillColor(GREEN);
        window.draw(bird);
        window.draw(topPipe);
        window.draw(bottomPipe);

        drawText(window, "FLAPPY BIRD  |  Score: " +
                 std::to_string(score), 10, 10, 20);
        if (over)
            drawText(window, "GAME OVER - Press R to restart",
                     180, 300, 25, RED);
    }

    void handleKey(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Key::Space) flap();
        if (key == sf::Keyboard::Key::R && over) reset();
    }
};

// ============================================================
// 6. Doodle Jump
// ============================================================

class DoodleGame : public Game
{
    sf::RectangleShape player{{32.f, 32.f}};
    std::vector<sf::RectangleShape> platforms;
    float velocityY = -400.f;
    float cameraOffset = 0.f;
    bool over = false;
    int score = 0;

public:
    std::string name() const override { return "Doodle Jump"; }

    void reset() override
    {
        player.setPosition({384.f, 500.f});
        velocityY = -400.f;
        cameraOffset = 0.f;
        score = 0;
        over = false;
        platforms.clear();

        for (int i = 0; i < 9; ++i)
        {
            sf::RectangleShape platform({85.f, 12.f});
            platform.setPosition({static_cast<float>(std::rand() % 700),
                                  560.f - i * 70.f});
            platform.setFillColor(GREEN);
            platforms.push_back(platform);
        }
    }

    void update(float dt) override
    {
        if (over) return;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            player.move({-280.f * dt, 0.f});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            player.move({280.f * dt, 0.f});

        sf::Vector2f p = player.getPosition();
        if (p.x < -32.f) player.setPosition({800.f, p.y});
        if (p.x > 800.f) player.setPosition({-32.f, p.y});

        velocityY += 850.f * dt;
        player.move({0.f, velocityY * dt});

        p = player.getPosition();

        if (velocityY > 0.f)
        {
            for (auto& platform : platforms)
            {
                sf::Vector2f q = platform.getPosition();
                if (overlaps(p.x, p.y, 32, 32, q.x, q.y, 85, 12) &&
                    p.y + 32.f - velocityY * dt <= q.y + 5.f)
                {
                    velocityY = -430.f;
                    break;
                }
            }
        }

        if (p.y < 250.f)
        {
            float shift = 250.f - p.y;
            player.move({0.f, shift});
            cameraOffset += shift;
            score = std::max(score, static_cast<int>(cameraOffset / 10.f));

            for (auto& platform : platforms)
                platform.move({0.f, shift});
        }

        for (auto& platform : platforms)
        {
            if (platform.getPosition().y > 610.f)
            {
                platform.setPosition({
                    static_cast<float>(std::rand() % 700),
                    static_cast<float>(-20 - std::rand() % 100)
                });
            }
        }

        if (player.getPosition().y > 600.f) over = true;
    }

    void draw(sf::RenderWindow& window) override
    {
        player.setFillColor(YELLOW);
        window.draw(player);
        for (auto& platform : platforms) window.draw(platform);

        drawText(window, "DOODLE JUMP  |  Score: " +
                 std::to_string(score), 10, 10, 20);
        if (over)
            drawText(window, "GAME OVER - Press R to restart",
                     180, 300, 25, RED);
    }

    void handleKey(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::Key::R && over) reset();
    }
};

// ============================================================
// Main application and game-selection menu
// ============================================================

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        return 1;
    }
    gameFont = &font;

    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "OOP Multi-Game Engine - SFML 3"
    );
    window.setFramerateLimit(60);

    std::vector<std::unique_ptr<Game>> games;
    games.push_back(std::make_unique<SnakeGame>());
    games.push_back(std::make_unique<PongGame>());
    games.push_back(std::make_unique<BreakoutGame>());
    games.push_back(std::make_unique<SpaceInvadersGame>());
    games.push_back(std::make_unique<FlappyGame>());
    games.push_back(std::make_unique<DoodleGame>());

    int selectedGame = -1;
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = std::min(clock.restart().asSeconds(), 0.05f);

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* keyEvent =
                    event->getIf<sf::Event::KeyPressed>())
            {
                sf::Keyboard::Key key = keyEvent->code;

                if (key == sf::Keyboard::Key::Escape)
                {
                    if (selectedGame != -1)
                        selectedGame = -1;
                    else
                        window.close();
                }
                else if (selectedGame == -1)
                {
                    if (key >= sf::Keyboard::Key::Num1 &&
                        key <= sf::Keyboard::Key::Num6)
                    {
                        selectedGame =
                            static_cast<int>(key) -
                            static_cast<int>(sf::Keyboard::Key::Num1);

                        games[selectedGame]->reset();
                    }
                }
                else
                {
                    if (auto* g = dynamic_cast<SnakeGame*>(
                            games[selectedGame].get()))
                        g->handleKey(key);

                    if (auto* g = dynamic_cast<BreakoutGame*>(
                            games[selectedGame].get()))
                        g->handleKey(key);

                    if (auto* g = dynamic_cast<SpaceInvadersGame*>(
                            games[selectedGame].get()))
                        g->handleKey(key);

                    if (auto* g = dynamic_cast<FlappyGame*>(
                            games[selectedGame].get()))
                        g->handleKey(key);

                    if (auto* g = dynamic_cast<DoodleGame*>(
                            games[selectedGame].get()))
                        g->handleKey(key);
                }
            }
        }

        window.clear(BACKGROUND);

        if (selectedGame == -1)
        {
            drawText(window, "OOP MULTI-GAME ENGINE", 150, 65, 34, YELLOW);
            drawText(window, "Select a game using keys 1 to 6", 190, 125, 22);

            const std::string names[] = {
                "1 - Snake",
                "2 - Pong",
                "3 - Breakout",
                "4 - Space Invaders",
                "5 - Flappy Bird",
                "6 - Doodle Jump"
            };

            for (int i = 0; i < 6; ++i)
                drawText(window, names[i], 280, 195.f + i * 48.f,
                         25, WHITE);

            drawText(window, "ESC - Quit | ESC inside a game - Menu",
                     145, 540, 18);
        }
        else
        {
            games[selectedGame]->update(dt);
            games[selectedGame]->draw(window);

            drawText(window, "ESC: Menu", 680, 570, 16);
        }

        window.display();
    }

    return 0;
}
