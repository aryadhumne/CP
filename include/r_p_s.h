#ifndef r_p_s_h
#define r_p_s_h
#include <iostream>

class Rock
{
public:
    std::string rock;
    int val;
    Rock();
};

class Paper
{
public:
    std::string paper;
    int val;
    Paper();
};

class Scissor
{
public:
    std::string scissor;
    int val;
    Scissor();
};

class r_p_s : public Rock, Paper, Scissor
{
    char Play;
    int score;

public:
    r_p_s();  // constructor empty default
    ~r_p_s(); // destructor empty pure
    int play();
};

#endif