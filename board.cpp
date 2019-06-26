#include "board.h"
#include <algorithm>    // std::shuffle
#include <stdlib.h>     /* srand, rand */
#include <time.h>

static int diff(const int& i1, const int& i2)
{
    return i1 > i2 ? i1 - i2 : i2 -i1;
}

Board::Board(const int& L)
:bound(L)
,lastMove(move)
{
    for(int row=1; row<8; ++row)
    {
        for(int col=1; col<6; ++col)
        {
            theGrid[row][col].occupied = 0;
			theGrid[row][col].row = row;
			theGrid[row][col].col = col;
            Node & toSet = theGrid[row][col];
            for (int dir = 0; dir < 8; ++dir)
            {
                const int ver = (dir == 7 || dir < 2) - (2 < dir && dir < 6);
                const int hor = (0 < dir && dir < 4) - (4 < dir);
                toSet.next[dir] = &theGrid[row + ver][col + hor];
            }
            toSet.decreaseDepth = (diff(4, row) <= 1 && diff(3, col) <= 1) ? 0 : -1;
        }
    }
    for(int id = 0; id <5; ++id)
    {
		collectionOfPlayer[id] = &theGrid[1][id+1];
		collectionOfPlayer[id]->occupied = -id-1;
		collectionOfProgram[id] = &theGrid[7][id+1];
		collectionOfProgram[id]->occupied = id+1;
    }
    collectionOfPlayer[5] = nullptr;
    collectionOfProgram[5] = nullptr;
    WIN = &theGrid[4][3];
    srand(time(NULL));
}

Board::~Board()
{
    std::cout<<"Finish"<<std::endl;
}

void Board::shuffle()
{
	//--- Shuffle OFF ---//
	return;
	//------------------//
	for (int index = 0; index < 5; ++index)
	{
		int id = rand() % 5;
		if (id != index)
		{
			std::swap( *(collectionOfProgram + index), *(collectionOfProgram + id));
		}
	}
}
bool Board::makeStep(const int& id, const int& dir, Step& step)
{
    Node ** stone = nullptr;
    if(id < 0)
    {
        stone = collectionOfPlayer-(id+1);
    }
    return stone ? makeStep(stone, dir, step) : false;
}

bool Board::makeStep(Node ** stone, const int& dir, Step& step)
{
    step.stone = stone;
    step.place = *stone;
    while(step.place->next[dir]->occupied == 0)
    {
        step.place = step.place->next[dir];
    }
    return *(step.stone) != step.place;
}

void Board::storeStep(Step S)
{
    S.revertableStep();
    *(++lastMove) = S;
}

void Board::undoStep()
{
	if (isStarted())
	{
		(lastMove--)->revertableStep();
	}
}

void Board::redoStep()
{
    if((lastMove + 1)->isValid())
    {
        (++lastMove)->revertableStep();
    }
}

static inline void NL() { std::cout << std::endl << '|'; }
void Board::show() const
{
    const char* separator = "---- ";
    const char* empty     = "    |";
    for(int row = 7;  row > 0; --row)
    {
        NL();
        for(int col = 1; col < 6; ++col)
        {
            std::cout<<separator;
        }        NL();
        for(int col = 1; col < 6; ++col)
        {
            const Node *field = &(theGrid[row][col]);
            const int CH = field->occupied;
            if(field == WIN && CH == 0)
            {
                std::cout<<" () |";
                continue;
            }
            if(CH < 0)
            {
                std::cout<<' '<<'A'<<(-1*CH)<<" |";
            }
            else if(CH > 0)
            {
                std::cout<<' '<<'X'<<(CH)<<" |";
            }
            else
            {
                std::cout<<empty;
            }
        }
    }
    NL();
    for(int col = 1; col < 6; ++col)
    {
        std::cout << separator;
    }
	NL();
	for (int i = 0; i < numberOfUsabelSteps; ++i)
	{
		std::cout << i << ". ";  usableSteps[i].printData();
	}
}

Step Board::randomStep()
{
	Step allSteps[40];
	int stepID = 0;
	Step test;
	for (Node** nextStone = collectionOfProgram; *nextStone; ++nextStone)
	{
		for (int direction = 0; direction < 8; ++direction)
		{
			if (makeStep(nextStone, direction, test))
			{
				allSteps[stepID++] = test;
			}
		}
	}
	return allSteps[rand() % stepID];
}
Board::Result Board::seek(Turn T, const int& depth, const bool& fastCheck)
{
	bool fail = (depth != 0);
	Step test;
	Node ** setOfStones = (T == YOURS) ? collectionOfPlayer : collectionOfProgram;
	Turn nextTurn = (T == YOURS) ? MINE : YOURS;
	for (Node** nextStone = setOfStones; *nextStone; ++nextStone)
	{
		for (int direction = 0; direction < 8; ++direction)
		{
			if (makeStep(nextStone, direction, test))
			{
				if (isWinnerStep(test))
				{
					return WON;
				}
				if (depth == 0)
				{
					continue;
				}
				test.revertableStep();
				const Result tip = seek(nextTurn, depth - 1, !fail);
				test.revertableStep();
				switch (tip)
				{
				case UNSURE:
					if (fastCheck)
					{
						return UNSURE;
					}
					fail = false;
					break;
				case LOST:
					return WON;
				case WON:
					break;
				default:
					break;
				};
			}
		}
	}
	return fail ? LOST : UNSURE;
}
void Board::getUsableSteps()
	{
	numberOfUsabelSteps = 0;
	Step test;
	for (Node** nextStone = collectionOfProgram; *nextStone; ++nextStone)
	{
		for (int direction = 0; direction < 8; ++direction)
		{
			if (makeStep(nextStone, direction, test))
			{
				if (isWinnerStep(test))
				{
					numberOfUsabelSteps = 0;
					usableSteps[numberOfUsabelSteps++] = test;
					return;
				}
				if (level == 0)
				{
					continue;
				}
				test.revertableStep();
				const Result tip = seek(YOURS, level - 1);
				test.revertableStep();
				switch (tip)
				{
				case UNSURE:
					usableSteps[numberOfUsabelSteps++] = test;
					break;
				case LOST:
					numberOfUsabelSteps = 0;
					usableSteps[numberOfUsabelSteps++] = test;
					return;
				case WON:
					break;
				default:
					break;
				}
			}
		}
	}
}
Step Board::getStep()
{
	shuffle();
	int stepID = 0;
	for (level = 0; level <= bound; level += 2)
	{
		getUsableSteps();
		if (numberOfUsabelSteps == 1)
		{
			return usableSteps[0];
		}
	}
	return numberOfUsabelSteps > 0 ? usableSteps[rand() % numberOfUsabelSteps] : randomStep();
}