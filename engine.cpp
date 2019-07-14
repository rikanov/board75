#include "engine.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>

Engine::Engine(const int& S, const int& L)
    :Board(S)
    ,_bound(L)
    ,_level(0)
{
}

Engine::~Engine()
{
}

bool Engine::makeStep(const int& id, const int& dir, Step& step)
{
    Node ** stone = nullptr;
    if(id < 0)
    {
        stone = __collectionOfPlayer-(id+1);
    }
    return stone ? makeStep(stone, dir, step) : false;
}

bool Engine::makeStep(Node ** stone, const int& dir, Step& step)
{
    if ( (*stone)->next[dir]->occupied != 0)
    {
        return false;
    }
    step.stone = stone;
    step.place = *stone;
    while(step.place->next[dir]->occupied == 0)
    {
        step.place = step.place->next[dir];
    }
    return true;
}

void Engine::storeStep(Step S)
{
    S.revertableStep();
    *(++_lastMove) = S;
}

void Engine::undoStep()
{
    if (isStarted())
    {
        (_lastMove--)->revertableStep();
    }
}

void Engine::redoStep()
{
    if((_lastMove + 1)->isValid())
    {
        (++_lastMove)->revertableStep();
    }
}

static inline void NL() {
    std::cout << std::endl << '|';
}
void Engine::show() const
{
    const char* separator = "---- ";
    const char* empty     = "    |";
    for(int row = _rows;  row > 0; --row)
    {
        NL();
        for(int col = 1; col <= _cols; ++col)
        {
            std::cout<<separator;
        }
        NL();
        for(int col = 1; col <= _cols; ++col)
        {
            const Node *field = &(__theGrid[row][col]);
            const int CH = field->occupied;
            if(field == _WINNER_SPOT && CH == 0)
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
    for(int col = 1; col <= _cols; ++col)
    {
        std::cout << separator;
    }
    NL();
    std::cout << _level <<  ':'  <<_numberOfUsableSteps;
}

Engine::Result Engine::seek0(Engine::Turn T, const bool& fast_check)
{
    Step test;
    Node ** setOfStones = (T == YOUR_TURN) ? __collectionOfPlayer : __collectionOfProgram;
    if (fast_check)
    {
        for (Node** nextStone = setOfStones; *nextStone; ++nextStone)
        {
            for (int direction = 0; direction < 8; ++direction)
            {
                if (makeStep(nextStone, direction, test) && isWinnerStep(test))
                {
                    return WON;
                }
            }
        }
    }
    else
    {
        for (Node** nextStone = setOfStones; *nextStone; ++nextStone)
        {
            for (int direction = 0; direction < 8; ++direction)
            {
                if (makeStep(nextStone, direction, test) )
                {
                    return isWinnerStep(test) ? WON : UNSURE;
                }
            }
        }
    }
    return UNSURE;
}

Engine::Result Engine::seek(Turn T, const int& depth, const bool& fast_check)
{
    if (depth == 0)
    {
        return seek0(T,fast_check);
    }
    bool fail = true;
    Step test;
    Node ** setOfStones = (T == YOUR_TURN) ? __collectionOfPlayer : __collectionOfProgram;
    Turn nextTurn = (T == YOUR_TURN) ? MY_TURN : YOUR_TURN;
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
                    if (fast_check)
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

void Engine::getUsableSteps()
{
    bool fastCheck = false;
    _numberOfUsableSteps = 0;
    for(int id =  getAllowedSteps() ; id > 0; --id)
    {
        Step test = __allowedSteps[id - 1]; 
        if (isWinnerStep(test))
        {
            _numberOfUsableSteps = 1;
            __usableSteps[0] = test;
            return;
        }
        if (_level == 0)
        {
            __usableSteps[_numberOfUsableSteps++] = test;
            continue;
        }
        test.revertableStep();
        const Result tip = seek(_getStepsForAi ? YOUR_TURN : MY_TURN, _level - 1, fastCheck);
        test.revertableStep();
        switch (tip)
        {
        case UNSURE:
            if ( !fastCheck)
            {
                __usableSteps[_numberOfUsableSteps++] = test;
                fastCheck = (_numberOfUsableSteps > 2);
            }
            break;
        case LOST:
            _numberOfUsableSteps = 1;
            __usableSteps[0] = test;
            return;
        case WON:
            break;
        default:
            break;
        }      
    }
}

Step Engine::getStep()
{
    Step bestFound;
    Step rescueSteps[MAX_NUMBER_OF_STONES * 8];
    int numberOfRescuedSteps = 0;
    for (_level = 0; _level <= _bound; _level += 1)
    {
        getUsableSteps();
        if (_numberOfUsableSteps == 1)
        {
            bestFound = __usableSteps[0];
            break; // don't think too much, an obviuos step is coming. Either winner or survivor
        }
        if (_numberOfUsableSteps == 0)
        {
            bestFound = rescueSteps[rand() % numberOfRescuedSteps];
            break; // we lost. try to rescue the game
        }
        // continue the searching and store the last known 'non-loser' steps for rescueing
        for (numberOfRescuedSteps = 0; numberOfRescuedSteps < _numberOfUsableSteps ; ++numberOfRescuedSteps)
        {
            rescueSteps[numberOfRescuedSteps] = __usableSteps[numberOfRescuedSteps];
        }
    }
    if (_level > _bound /*obvious step not found*/ )
    {
        bestFound =  __usableSteps[rand() % _numberOfUsableSteps] ;
    }
    return bestFound;
}

int Engine::getAllowedSteps()
{
    int count = 0;
    Step next;
    for (Node** nextStone = _getStepsForAi ?  __collectionOfProgram : __collectionOfPlayer; *nextStone; ++nextStone)
    {
        for (int direction = 0; direction < 8; ++direction)
        {
            if (makeStep(nextStone, direction, next))
            {
                __allowedSteps[count++] = next;
            }
        }
    }
    // shuffle
    for (int shuffle = count; shuffle; )
    {
        const int i1 = rand() % count;
        const int i2 = rand() % count;
        if ( i1 != i2)
        {
            std::swap(__allowedSteps[i1], __allowedSteps[i2]);
        }
        else
        {
            --shuffle;
        }
    }
    return count ;
}
