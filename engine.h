#ifndef BOARD_HEADER
#define BOARD_HEADER

#include "game.h"

class Engine: public Game
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
    bool _getStepsForAi = true;
    Result seek(Turn T, const int& depth, const bool& fast_check = false);
    Result seek0(Turn, const bool&);
	
	int getAllowedSteps();
	void getUsableSteps();
    
public:
    Engine(const int&, const int&);
    ~Engine();
    void swapOpponents() {
        std::swap(__collectionOfPlayer, __collectionOfProgram);
    }
    const int& boundLevel() const {
        return _bound;
    }
    void boundLevel(const int& bound) {
        _bound = bound;
    }
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
};

#endif

