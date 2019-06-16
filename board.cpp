#include "board.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>

static inline int diff(const int& i1, const int& i2)
{
    return i1 > i2 ? i1 - i2 : i2 -i1;
}
Board::Board(const int& L)
:level(L)
,lastMove(move)
{
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
            toSet.decreaseDepth = (diff(4, row) <= 1 && diff(3, col) <= 1) ? 0 : -1;
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

#define NL std::cout<<std::endl<<'|';
void Board::show() const
{
    const char* separator = "---- ";
    const char* empty     = "    |";
    for(int row = 7;  row > 0; --row)
    {
        NL
        for(int col = 1; col < 6; ++col)
        {
            std::cout<<separator;
        }        NL
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
    NL
    for(int col = 1; col < 6; ++col)
    {
        std::cout<<separator;
    }
    std::cout<<std::endl;
}

Step Board::getStep()
{
    /*---- Crucial part ----*/
    forseen = level + 16;
    /*----------------------*/
    
    Step best, test;
    Step bestCollection[40], allSteps[40];
    int collectionID = 0, stepID = 0;
    int highlight = -forseen;
    int eval = -forseen;
    
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
        eval = evalPlayer(highlight, level + test.decreaseDepth());
        test.revertableStep();
        if (highlight < eval)
        {
            collectionID = 1;
            bestCollection[0] = test;
            highlight = eval;
        }
        else if (highlight == eval )
        {
            bestCollection[collectionID++] = test;
        }
    }
    return bestCollection[collectionID ? rand()%collectionID : 0];
}

int Board::evalPlayer(const int& bound, const int& depth)
{
    if (depth == 0)
    {
        return 0; //  heuristicValue(); // ToDo, heuristic evaulation might be required here
    }
    Step test;
    Step allSteps[40];
    int stepID = 0;
    --forseen;
    int highlight = forseen;
    int eval = forseen;
    for (Node ** next = playerStone; *next; ++next)
    {
        for (int dir = 0; dir < 8; ++dir)
        {
            if (makeStep(next, dir, test))
            {
                if (isWinnerStep(test))
                {
                    return -(forseen++);
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
        return forseen++;
    }
    for(int id = 0; id < stepID; ++id)
    {
        test = allSteps[id];
        test.revertableStep(); 
        eval = evalProgram(highlight, depth + test.decreaseDepth());
        test.step();
        if (eval <= bound)
        {
            ++forseen;
            return eval;
        }
        if (highlight > eval)
        {
            highlight = eval;
        }
    }
    ++forseen;
    return highlight;
}

int Board::evalProgram(const int& bound, const int& depth)
{
    if (depth == 0)
    {
        return 0; //  heuristicValue(); // ToDo, heuristic evaulation might be required here
    }
    Step test;
    Step allSteps[40];
    int stepID = 0;
    --forseen;
    int highlight = -forseen;
    int eval = -forseen;
    for (Node ** next = programStone; *next; ++next)
    {
        for (int dir = 0; dir < 8; ++dir)
        {
            if (makeStep(next, dir, test))
            {
                if (isWinnerStep(test))
                {
                    return forseen++ ;
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
        return -(forseen++);
    }
    for(int id = 0; id < stepID; ++id)
    {
        test = allSteps[id];
        test.revertableStep();
        eval = evalPlayer(highlight, depth + test.decreaseDepth());
        test.step();
        if (eval >= bound)
        {
            ++forseen;
            return eval;
        }
        if (highlight < eval)
        {
            highlight = eval;
        }
    }
    ++forseen;
    return highlight;
}

int Board::heuristicValue() 
{
    // ToDo 
    return 0;
}
