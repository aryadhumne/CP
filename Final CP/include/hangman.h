#ifndef hangman_h
#define hangman_h
#include <string>

class hangman
{
private:
     int lives = 7, index, score;
     const std::string words[30] = {
         "apple", "banana", "cherry", "grape", "orange",
         "watermelon", "strawberry", "blueberry", "pineapple", "mango", "kiwi", "peach", "pear", "plum", "pomegranate",
         "sprinkles", "tornado", "whistle", "sphinx", "unicorn", "jellyfish", "dandelion", "butterscotch", "sundial", "kaleidoscope", "zigzag", "whirlpool", "paradox", "spaghetti", "chandelier"};
     char input;
     const std::string logo = R"(
 _                                             
| |                                            
| |__   __ _ _ __   __ _ _ __ ___   __ _ _ __  
| '_ \ / _` | '_ \ / _` | '_ ` _ \ / _` | '_ \
| | | | (_| | | | | (_| | | | | | | (_| | | | |
|_| |_|\__,_|_| |_|\__, |_| |_| |_|\__,_|_| |_|
                    __/ |                      
                   |___/     
)";

     const std::string man[7] = {
         R"(
 +---+
 |	 |
 O	 |
/|\  |
/ \  |
     |
=========)",
         R"(
 +---+
 |	 |
 O	 |
/|\  |
/	 |
     |
=========)",
         R"(
 +---+
 |	 |
 O	 |
/|\  |
     |
     |
=========)",
         R"(
 
 +---+
 |	 |
 O	 |
/|	 |
     |
     |
=========)",
         R"(
 +---+
 |	 |
 O	 |
 |	 |
     |
     |
=========)",
         R"(
 +---+
 |	 |
 O	 |
     |
     |
     |
=========)",
         R"(
 +---+
 |	 |
     |
     |
     |
     |
=========
)"};

public:
     hangman();
     ~hangman();
     int play();
};

#endif