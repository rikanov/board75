#ifndef GAME_HEADER__H
#define GAME_HEADER__H

#include "board.h"

class Game: public Board
{
protected:

	int _numberOfUsableSteps;
	Step __usableSteps[MAX_NUMBER_OF_STONES * 8];
	Step __allowedSteps[MAX_NUMBER_OF_STONES * 8];

	// Store steps for UI (undo, redo)
	Step * __moveHistory, *_currentMove, *_lastMove;

public:
	Game(const int& S);
	~Game();
	void reset();
	bool makeStep(const int& id, const int&, Step&);
	bool makeStep(Node **, const int&, Step&);
	void storeStep(Step S);
	void undoStep();
	void redoStep();
    Step reverseLast() {
        return *_currentMove;
    }
	bool isStarted() const {
		return _currentMove != __moveHistory + 1;
	}
	bool isTheLastMove() const {
        return _currentMove == _lastMove;
    }
	int stepCount() const {
		return (_currentMove - __moveHistory);
	}
	bool isFinished() const {
		return _WINNER_SPOT->occupied != 0;
	}
	bool isWinnerStep(const Step& S) const {
		return S.place == _WINNER_SPOT;
	}
};
#endif // !GAME_HEADER__H

