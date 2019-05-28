#ifndef BOARD_HEADER
#define BOARD_HEADER

#include <iostream>

static const int MAX_NUMBER_OF_MOVES = 200;
static const int WALL = -7;
struct Node
{
    Node* next[8] = {};
    int occupied = WALL;
    int value = 0;
};

struct Step
{
    Node ** stone;
    Node * place;
    Node * tmp;
    
    public:
    Step(): stone(nullptr), place(nullptr) {}
    Step(Node ** s, Node * p): stone(s), place(p) {}
    Step(const Step&) = default;
   
    void step()
    {
        place->occupied = (*stone)->occupied; // put down
        (*stone)->occupied = 0;              // and pick up
        *stone = place;
    }
    void revertableStep()
    {
        place->occupied = (*stone)->occupied; // put down
        (*stone)->occupied = 0;              // and pick up
        std::swap(*stone, place);
    }
};

struct Position
{
    int row, col;
    
    public:
    Position(): row(0), col(0) {}
    Position(const int& r, const int& c): row(r), col(c) {}
    Position(const Position&) = default;  
};

class Board
{
    protected:
    
    const Node* WIN;
    const int level;

    Node theGrid[9][7];
    Node * playerStone[6];
    Node * programStone[6];
    Step move[MAX_NUMBER_OF_MOVES], *lastMove;
    
    Step ** stepBuffer;
    
    int evalPlayer(const int&, const int&);
    int evalProgram(const int&, const int&);
    int heuristicValue();

    public:
    Board(const int&);
    ~Board();
    bool makeStep(const int& id, const int&, Step& );
    bool makeStep(Node **, const int&, Step& );
    bool isStarted() const {return lastMove != move;}
    void storeStep(Step S);
    void undoStep();
    void redoStep();

    Step getStep();
    bool isWinnerStep(const Step& S) const { return S.place == WIN; }
    
    void show() const;
};

#endif

