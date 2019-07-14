#ifndef BOARD_HEADER
#define BOARD_HEADER

#include <iostream>

static const int MAX_NUMBER_OF_MOVES = 200;
static const int MAX_NUMBER_OF_STONES = 10;
static const int MAX_HEURISTIC_DEPTH = 10;
static const int WALL = -7;
struct Node
{
    Node* next[8] = {};
    int occupied = WALL;
    int decreaseDepth = -1;
    int row, col;
};

struct Step
{
    Node ** stone;
    Node * place;

public:
    Step(): stone(nullptr), place(nullptr) {}
    Step(Node ** s, Node * p): stone(s), place(p) {}
    Step(const Step&) = default;
    bool isValid() const {
        return stone != nullptr && place != nullptr;
    }
    void revertableStep()
    {
        place->occupied = (*stone)->occupied; // put down
        (*stone)->occupied = 0;              // and pick up
        std::swap(*stone, place);
    }

    int decreaseDepth() const
    {
        return (*stone)->decreaseDepth;
    }

    void printData() const
    {
        printf("X%d->(%d, %d) ", (*stone)->occupied, place->row, place->col);
    }
};

class Board
{
public:
    enum Result
    {
        LOST = -1,
        UNSURE = 0,
        WON = 1
    };
    enum Turn
    {
        MY_TURN,
        YOUR_TURN
    };
protected:

    int __pow[MAX_HEURISTIC_DEPTH];
    bool _heuristic;
    const Node* _WINNER_SPOT;
     int _bound;
    int _level;

    // Board grid [rows+2][columns+2]
    Node ** __theGrid;
    const int _rows;
    const int _cols;

    // Stones os players [columns]
    Node ** __collectionOfPlayer;
    Node ** __collectionOfProgram;
    
    // Store steps for UI (undo, redo)
    Step __move[MAX_NUMBER_OF_MOVES], *_lastMove;

    Step __usableSteps[MAX_NUMBER_OF_STONES * 8];
    Step __allowedSteps[MAX_NUMBER_OF_STONES * 8];
    bool _getStepsForAi = true;
    int _numberOfUsableSteps;
    Result seek(Turn T, const int& depth, const bool& fast_check = false);
    Result seek0(Turn, const bool&);
    
    void getUsableSteps();
    int getAllowedSteps();
    
public:
    Board(const int&, const int&);
    ~Board();
    void swapOpponents() {
        std::swap(__collectionOfPlayer, __collectionOfProgram);
    }
    const Node * getStone(const int & ID) const {
        return ID > 0 ? __collectionOfProgram[ID - 1] : __collectionOfPlayer[-ID - 1];
    }
    const int& boundLevel() const {
        return _bound;
    }
    void boundLevel(const int& bound) {
        _bound = bound;
    }
    bool makeStep(const int& id, const int&, Step& );
    bool makeStep(Node **, const int&, Step& );
    bool isStarted() const {
        return _lastMove != __move;
    }
    void storeStep(Step S);
    void undoStep();
    void redoStep();
    void seekerSwap() {
        _getStepsForAi = !_getStepsForAi;
    }
    void getStepsForAi(const bool& t) {
        _getStepsForAi = t;
    }
    bool getStepsForAi() const {
        return _getStepsForAi;
    }
    Step getStep();
    bool isWinnerStep(const Step& S) const {
        return S.place == _WINNER_SPOT;
    }
    
    bool isFinished() const {
        return _WINNER_SPOT->occupied;
    }
    
    void reset();
    void show() const;
};

#endif

