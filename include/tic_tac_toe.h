#ifndef tic_tac_toe_h
#define tic_tac_toe_h

class tic_tac_toe
{
    char board[3][3], user, comp, user2;
    int score;

public:
    tic_tac_toe();
    ~tic_tac_toe();
    void display();
    bool clearBoard();
    bool initialize(int pos, char symbol);
    bool checkIfWin(char symbol);
    void compplay();
    int play(char u);
    int play(void);
};

#endif