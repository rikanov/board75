#ifndef BOARD_HEADER
#define BOARD_HEADER

#include <iostream>

static const int MAX_NUMBER_OF_MOVES = 200;
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
	bool isValid() const { return stone != nullptr && place != nullptr; }
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
public:
	enum Result
	{
		LOST = -1,
		UNSURE = 0,
		WON = 1
	};
	enum Turn
	{
		MINE,
		YOURS
	};
protected:

    const Node* WIN;
    const int bound; 
	int level;

    Node theGrid[9][7];
    Node * collectionOfPlayer[6];
    Node * collectionOfProgram[6];
    Step move[MAX_NUMBER_OF_MOVES], *lastMove;
    
	Step usableSteps[40]; int numberOfUsabelSteps;

	void shuffle();

	Result seek(Turn, const int&, const bool& fastCheck = false);
	Step randomStep();

    public:
    Board(const int&);
    ~Board();
    bool makeStep(const int& id, const int&, Step& );
    bool makeStep(Node **, const int&, Step& );
    bool isStarted() const {return lastMove != move;}
    void storeStep(Step S);
    void undoStep();
    void redoStep();

	void getUsableSteps();
	Step getStep();
    bool isWinnerStep(const Step& S) const { return S.place == WIN; }
    
    void show() const;
};

#endif

