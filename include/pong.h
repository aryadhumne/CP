#ifndef pong_h
#define pong_h

class Pong
{
    int width, height;
    int ballX, ballY;
    int ballDX, ballDY;
    int paddle1Y, paddle2Y;
    int paddleH;
    int score1, score2;
    int maxScore;

    void draw();
    void moveBall();
    bool checkPaddleHit();

public:
    Pong();
    ~Pong();
    int play();
};

#endif
