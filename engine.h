#ifndef BOARD_HEADER
#define BOARD_HEADER

#include "board.h"

class Engine: public Board
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
     int _bound;
    int _level;

    Step __usableSteps[MAX_NUMBER_OF_STONES * 8];
    Step __allowedSteps[MAX_NUMBER_OF_STONES * 8];
    bool _getStepsForAi = true;
    int _numberOfUsableSteps;
    Result seek(Turn T, const int& depth, const bool& fast_check = false);
    Result seek0(Turn, const bool&);
    
    void getUsableSteps();
    int getAllowedSteps();
    
public:
    Engine(const int&, const int&);
    ~Engine();
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
    void show() const;
};

#endif

