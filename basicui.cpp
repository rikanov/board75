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

#include "basicui.h"
#include <unistd.h>
/*
#ifdef _WIN32
	#include <windows.h>
	static inline void CLR() { system("cls"); }
	static inline void sleep(unsigned milliseconds)
	{
		Sleep(milliseconds);
	}
#else
	#include <unistd.h>
	static inline void CLR() { stdd::cout << std::endl; system("clear"); }
	static inline void sleep(unsigned milliseconds)
	{
		usleep(milliseconds * 1000); // takes microseconds
	}
#endif //*/
static inline void CLR() {
    system("clear");
}

BasicUI::BasicUI(const int& size, const int& level, const bool& player_begin, const bool& auto_play)
    :_autoPlay(auto_play)
{
    _board = new Board(size, level);
    _isPlayerTurn = player_begin;
}

BasicUI::~BasicUI()
{
    delete _board;
}

void BasicUI::start()
{
    CLR();
    _board->show();
    for(bool finish = false; !finish;)
    {
        if ( _isPlayerTurn )
        {
            finish = _autoPlay ? autoPlay(9) : yourTurn();
            _isPlayerTurn = false;
        }
        else
        {
            finish = _autoPlay ? autoPlay(8) :  myTurn();
            _isPlayerTurn = true;
        }
    }
}

bool BasicUI::myTurn()
{
    bool win = false;
    Step st = _board->getStep();
    _board->storeStep(st);
    CLR();
    _board->show();
    if ( _board->isWinnerStep(st))
    {
        std::cout << "AI WON !!!" << std::endl;
        win = true;
    }
    return win;
}

bool BasicUI::yourTurn()
{
    bool win = false;
    Step st;
    for (bool valid = false; !valid;)
    {
        std::cout << std::endl << "> " << std::flush;
        std::cin.clear();
        std::cin.ignore(8, '\n');
        int id, dir;
        std::cin >> id >> dir;
        if (id == 0 && dir < 0  && _board->isStarted())
        {
            _board->undoStep();
            CLR();
            _board->show();
            sleep(1);
            _board->undoStep();
            CLR();
            _board->show();
            continue;
        }
        else if (id == 0 && dir > 0)
        {
            _board->redoStep();
            CLR();
            _board->show();
            sleep(1);
            _board->redoStep();
            CLR();
            _board->show();
            continue;
        }
        else
        {
            valid = _board->makeStep(id, dir, st);
        }

        if (!valid)
        {
            std::cout << "ERR: " << id << ':' << dir << std::endl;
        }
    }
    _board->storeStep(st);
    CLR();
    _board->show();
    if ( _board->isWinnerStep(st))
    {
        std::cout << "YOU WON !!!" << std::endl;
        win = true;
    }
    return win;
}

bool BasicUI::autoPlay(const int& level)
{
    bool win = false;
    if (level > 0)
    {
        _board->boundLevel(level);
    }
    Step st = _board->getStep();
    _board->storeStep(st);
    CLR();
    _board->show();
    if ( _board->isWinnerStep(st))
    {
        std::cout << "THE" << (_isPlayerTurn ?  " HEURISTIC " : " SIMPLIER") << " AI WON !!!" << std::endl;
        win = true;
    }
    _board->swapOpponents();
    return win;
}


