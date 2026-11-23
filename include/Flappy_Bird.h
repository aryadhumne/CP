#ifndef Flappy_Bird_h
#define Flappy_Bird_h
#include <string>
#include <vector>

class FlappyBird
{
    int birdX, birdY;
    int emptyLeft;
    std::string bird;
    std::vector<char> reach;
    int top[20], bottom[20];
    void *hOriginal;
    void *hGame;

public:
    FlappyBird();
    ~FlappyBird();
    void fly(int speed);
    int start(int speed);
    void clearConsole();
    bool drawPipes(int offset = 0, bool erase = false);
    bool generatePipes(int height, int from = 0);
};

#endif // Flappy_Bird_h