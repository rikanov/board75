#include "board.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>

static int diff(const int& i1, const int& i2)
{
    return i1 > i2 ? i1 - i2 : i2 -i1;
}

Board::Board(const int& S, const int& L)
    :_bound(L)
    ,_level(0)
    ,_rows(S + 2)
    ,_cols(S)
    ,__collectionOfPlayer(new Node* [_cols + 1])  /*null-terminated array*/
    ,__collectionOfProgram(new Node* [_cols + 1]) /*null-terminated array*/
{
    // n-th power values for heuristic deep sum
    __pow[0] = 1;
    for (int i = 1; i < _bound; ++i)
    {
        __pow[i] = __pow[i - 1] * S;
    }
    // --- Create grid --- //
    // allocate size
    __theGrid = new Node* [_rows + 2];
    for (int row = 0; row < _rows + 2; ++row)
    {
        __theGrid[row] = new Node[_cols + 2];
    }
    // set borders -> vertical
    for (int id = 1; id < _cols + 2; ++id)
    {
        __theGrid[0][id].occupied = WALL;
        __theGrid[_rows][id].occupied = WALL;
    }
    // set borders -> horizontal
    for (int id = 1; id < _rows + 2; ++id)
    {
        __theGrid[id][_cols].occupied = WALL;
        __theGrid[id][0].occupied = WALL;
    }
    // set auxiliary parameters and make topology
    for (int row = 1; row < _rows + 1; ++row)
    {
        for(int col = 1; col < _cols + 1; ++col)
        {
            __theGrid[row][col].occupied = 0;
            __theGrid[row][col].row = row;
            __theGrid[row][col].col = col;
            Node & toSet = __theGrid[row][col];
            for (int dir = 0; dir < 8; ++dir)
            {
                const int ver = (dir == 7 || dir < 2) - (2 < dir && dir < 6);
                const int hor = (0 < dir && dir < 4) - (4 < dir);
                toSet.next[dir] = &__theGrid[row + ver][col + hor];
            }
            toSet.decreaseDepth = (diff((_rows + 1) / 2, row) <= 1 && diff((_cols + 1) / 2, col) <= 1) ? 0 : -1;
        }
    }
    reset();
    // set the middle field of the board as the place of victory
    _WINNER_SPOT = &__theGrid[(_rows + 1) / 2][(_cols + 1) / 2];
    // --------------- //
}

void Board::reset()
{
    for (int row = 1; row < _rows + 1; ++row)
    {
        for(int col = 1; col < _cols + 1; ++col)
        {
            __theGrid[row][col].occupied = 0;
        }
    }
    for(int id = 0; id <_cols; ++id)
    {
        __collectionOfPlayer[id] = &__theGrid[1][id + 1];
        __collectionOfPlayer[id]->occupied = -id -1 ;
        __collectionOfProgram[id] = &__theGrid[_rows][id + 1];
        __collectionOfProgram[id]->occupied = id + 1;
    }
    // null-terminate arrays for the opponents' stones
    __collectionOfPlayer[_cols] = nullptr;
    __collectionOfProgram[_cols] = nullptr;

    // --- set step stacks for UI -- //
    _lastMove =  __move;
    // -------------- //
    srand(time(NULL));
}

Board::~Board()
{
    delete[] __collectionOfPlayer;
    delete[] __collectionOfProgram;
    std::cout<<"Finish"<<std::endl;
}

bool Board::makeStep(const int& id, const int& dir, Step& step)
{
    Node ** stone = nullptr;
    if(id < 0)
    {
        stone = __collectionOfPlayer-(id+1);
    }
    return stone ? makeStep(stone, dir, step) : false;
}

bool Board::makeStep(Node ** stone, const int& dir, Step& step)
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

void Board::storeStep(Step S)
{
    S.revertableStep();
    *(++_lastMove) = S;
}

void Board::undoStep()
{
    if (isStarted())
    {
        (_lastMove--)->revertableStep();
    }
}

void Board::redoStep()
{
    if((_lastMove + 1)->isValid())
    {
        (++_lastMove)->revertableStep();
    }
}

static inline void NL() {
    std::cout << std::endl << '|';
}
void Board::show() const
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

Board::Result Board::seek0(Board::Turn T, const bool& fast_check)
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

Board::Result Board::seek(Turn T, const int& depth, const bool& fast_check)
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

void Board::getUsableSteps()
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

Step Board::getStep()
{
    Step bestFound;
    Step rescueSteps[MAX_NUMBER_OF_STONES * 8];
    int numberOfRescuedSteps = 0;
    std::cout << std::endl;
    for (_level = 0; _level <= _bound; _level += 1)
    {
        std::cout << _level << std::flush;
        getUsableSteps();
        std::cout << " .. " << std::flush;
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

int Board::getAllowedSteps()
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
