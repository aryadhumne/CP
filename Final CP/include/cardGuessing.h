#ifndef cardGuessing_h
#define cardGuessing_h
#include <string>

class cardGuessing
{
private:
  const std::string symbols[4] = {R"(
  ,'`.
 (_,._)
   /\	)",

                                  R"(
  _  _ 
 ( `' )
  `.,' )",

                                  R"(
 /\
<  >
 \/ )",

                                  R"(
    _
  _(_)_ 
 (_)+(_)
    |	)"};

  char guessSym, cardSym;
  int guess, card, sym, lives, score;

public:
  cardGuessing();
  ~cardGuessing();
  int play();
  void setSymbol(int sym);
  bool checkMatch(char guessSym, int guess);
};

#endif