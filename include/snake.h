#ifndef snake_h
#define snake_h
#include <vector>
#include <utility>

class Snake
{
    int width, height;
    int dirX, dirY;
    int foodX, foodY;
    int score;
    std::vector<std::pair<int, int>> body;

    void generateFood();
    bool isOnBody(int x, int y);

public:
    Snake();
    ~Snake();
    int play();
};

#endif
