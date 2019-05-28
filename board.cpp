#include "board.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>

Board::Board(const int& L)
:level(L)
,lastMove(move)
{
    stepBuffer = new Step* [L+1];
    for(int i=0;i<=level;++i)
    {
        stepBuffer[i] = new Step[120];
    }
    for(int row=1; row<8; ++row)
    {
        for(int col=1; col<6; ++col)
        {
            theGrid[row][col].occupied = 0;
            Node & toSet = theGrid[row][col];
            for (int dir = 0; dir < 8; ++dir)
            {
                const int ver = (dir == 7 || dir < 2) - (2 < dir && dir < 6);
                const int hor = (0 < dir && dir < 4) - (4 < dir);
                toSet.next[dir] = &theGrid[row + ver][col + hor];
            }
            toSet.value = (4 - row)*(4 - row) - (3 - col)*(3 - col);
        }
    }
    for(int id = 0; id <5; ++id)
    {
        playerStone[id] = &theGrid[1][id+1];
        playerStone[id]->occupied = -id-1;
        programStone[id]= &theGrid[7][id+1];
        programStone[id]->occupied = id+1;
    }
    playerStone[5] = nullptr;
    programStone[5] = nullptr;
    WIN = &theGrid[4][3];
    srand(time(NULL));
}

Board::~Board()
{
    for(int i=0;i<=level;++i)
    {
        delete[] stepBuffer[i];
    }
    delete[] stepBuffer;
    std::cout<<"Finish"<<std::endl;
}

bool Board::makeStep(const int& id, const int& dir, Step& step)
{
    Node ** stone = nullptr;
    if(id < 0)
    {
        stone = playerStone-(id+1);
    }
    else if(id > 0)
    {
        stone = programStone+id-1;
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
    if(lastMove != move)
        (lastMove--)->revertableStep();
}

void Board::redoStep()
{
    if(lastMove->stone)
    {
        (++lastMove)->revertableStep();
    }
}

void Board::show() const
{
    for(int row = 8; row >= 0; --row)
    {
        std::cout<<std::endl;
        for(int col = 0; col < 7; ++col)
        {
            const int CH = theGrid[row][col].occupied;
            if(CH == WALL)
            {
                std::cout<<((col == 0 || col == 6) ? "#" : "##");
            }
            else if(CH < 0)
            {
                std::cout<<'A'<<(-1*CH);
            }
            else if(CH > 0)
            {
                std::cout<<'X'<<CH;
            }
            else
            {
                std::cout<<"  ";
            }
        }
    }
    std::cout<<std::endl;
}

Step Board::getStep()
{
    Step best, test;
    Step bestCollection[120], allSteps[120];
    int collectionID = 0, stepID = 0;
    int highlight = -level;
    int eval = -level;
    for (Node ** next = programStone; *next; ++next)
    {
        for (int dir = 0; dir < 8; ++dir)
        {
            if (makeStep(next, dir, test))
            {
                if (isWinnerStep(test))
                {
                    return test;
                }
                else
                {
                    allSteps[stepID++] = test;
                }
            }
        }
    }
    /* an extreme situation: no valid moves and it implies instant defeat */
    /* function will return with invalid step, it must be handled by UI  */
    for(int id = 0; id < stepID; ++id)
    {
        test = allSteps[id];
        test.revertableStep();
        eval = evalPlayer(highlight, level);
        test.revertableStep();
        if (highlight < eval)
        {
            collectionID = 1;
            bestCollection[0] = test;
            highlight = eval;
        }
        else if (highlight == eval )
        {
            bestCollection[collectionID] = test;
        }
    }
    return bestCollection[collectionID ? rand()%collectionID : 0];
}

int Board::evalPlayer(const int& bound, const int& deep)
{
    if (deep == 0)
    {
        return 0; //  heuristicValue(); // ToDo, heuristic evaulation might be required here
    }
    Step test;
    Step * allSteps = stepBuffer[deep];
    int stepID = 0;
    int highlight = deep;
    int eval = deep;
    for (Node ** next = playerStone; *next; ++next)
    {
        for (int dir = 0; dir < 8; ++dir)
        {
            if (makeStep(next, dir, test))
            {
                if (isWinnerStep(test))
                {
                    return -deep;
                }
                else
                {
                    allSteps[stepID++] = test;
                }
            }
        }
    }
    /* there is no valid move */
    if(stepID == 0)
    {
        return deep;
    }
    for(int id = 0; id < stepID; ++id)
    {
        test = allSteps[id];
        test.revertableStep();
        eval = evalProgram(highlight, deep-1);
        test.step();
        if (eval <= bound)
        {
            return eval;
        }
        if (highlight > eval)
        {
            highlight = eval;
        }
    }
    return highlight;
}

int Board::evalProgram(const int& bound, const int& deep)
{
    if (deep == 0)
    {
        return 0; //  heuristicValue(); // ToDo, heuristic evaulation might be required here
    }
    Step test;
    Step * allSteps = stepBuffer[deep];
    int stepID = 0;
    int highlight = -deep;
    int eval = -deep;
    for (Node ** next = programStone; *next; ++next)
    {
        for (int dir = 0; dir < 8; ++dir)
        {
            if (makeStep(next, dir, test))
            {
                if (isWinnerStep(test))
                {
                    return deep;
                }
                else
                {
                    allSteps[stepID++] = test;
                }
            }
        }
    }
    /* there is no valid move */
    if(stepID == 0)
    {
        return -deep;
    }
    for(int id = 0; id < stepID; ++id)
    {
        test = allSteps[id];
        test.revertableStep();
        eval = evalPlayer(highlight, deep-1);
        test.step();
        if (eval >= bound)
        {
            return eval;
        }
        if (highlight < eval)
        {
            highlight = eval;
        }
    }
    return highlight;
}

int Board::heuristicValue() 
{
    int retVal = 0;
    for (Node ** next = programStone; *next; ++next)
    {
        retVal += (*next)->value;
    }
    return 0;
}
