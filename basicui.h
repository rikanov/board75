/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2019  Robert Ikanov <email>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef BASICUI_H
#define BASICUI_H

#include "board.h"

class BasicUI
{
    Board * _board; 
    const bool _autoPlay;

    bool myTurn();
    bool yourTurn();
    bool _isPlayerTurn;
    
public:
    BasicUI(const int& size, const int& level, const bool& player_begin, const bool& autoPlay = false);
    ~BasicUI();
    void start();    
    bool autoPlay(const int& level = 0);
};

#endif // BASICUI_H
