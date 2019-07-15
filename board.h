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

#ifndef BOARD_H
#define BOARD_H

#include "str_step.h"

class Board
{
protected:
    const Node* _WINNER_SPOT;
    // Board grid [rows+2][columns+2]
    Node ** __theGrid;
    const int _rows;
    const int _cols;

    // Stones os players [columns]
    Node ** __collectionOfPlayer;
    Node ** __collectionOfProgram;
    
    // Store steps for UI (undo, redo)
    Step * __move, *_lastMove;
public:
    Board(const int&);
    virtual ~Board();
    void reset();
};

#endif // BOARD_H
