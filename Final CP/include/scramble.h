#ifndef scramble_h
#define scramble_h
#include <string>

class scramble
{
private:
    std::string words[30] = {
        "unicorn", "rainbow", "chocolate", "banana", "superhero", "marshmallow", "wizard", "penguin", "cupcake", "bubble", "jellybean", "bamboo", "zebra", "pumpkin", "lollipop", "flamingo", "ninja", "hamburger", "waffle", "giggle", "spaceship", "deadpool", "zeus", "vikings", "bubblegum", "waffle", "easter", "carcass", "titan", "hologram"};
    int word, lives, score;
    std::string scarmbled, guessed, selected;

public:
    scramble();
    ~scramble();
    int play();
};

#endif