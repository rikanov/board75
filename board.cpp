/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2019  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "board.h"

Board::Board(const int& S)
:_rows(S + 2)
,_cols(S)
,__move(nullptr)
{
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
        }
    }
    
    // --- Collections of stones --- /
    // allocate memory
    __collectionOfPlayer = new Node* [_cols + 1] ;     /*null-terminated array*/
    __collectionOfProgram = new Node* [_cols + 1];  /*null-terminated array*/
    // default positions
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
    delete __move; // purify it
    __move = new Step[MAX_NUMBER_OF_MOVES];
    _lastMove =  __move;
    // -------------- //
    srand(time(NULL));
}

Board::~Board()
{
    delete[] __collectionOfPlayer;
    delete[] __collectionOfProgram;
    delete __move;
}

