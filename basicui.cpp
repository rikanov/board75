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

BasicUI::BasicUI(const int& level, const bool& player_begin)
:teszt(new Board(level))
,isPlayerTurn(player_begin)
{
    
}

BasicUI::~BasicUI()
{
}

void BasicUI::start()
{
  Step st;
  system("clear");
  teszt->show();
  for(bool finish = false; !finish;)
  {
      if (isPlayerTurn)
      {
          finish = yourTurn();
          isPlayerTurn = false;
      }
      else
      {
          finish = myTurn();
          isPlayerTurn = true;
      }
  }
}

bool BasicUI::myTurn()
{
    bool win = false;
    Step st = teszt->getStep();
    teszt->storeStep(st);
    system("clear");
    teszt->show();
    if (teszt->isWinnerStep(st))
    {
        std::cout << "AI WON !!!" << std::endl;
        char a; std::cin >> a;
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
        int id, dir;
        std::cin >> id >> dir;
        valid = teszt->makeStep(id, dir, st);

        if (!valid)
        {
            std::cout << "ERR: " << id << ':' << dir << std::endl;
        }
    }
    teszt->storeStep(st);
    system("clear"); 
    teszt->show();
    if (teszt->isWinnerStep(st))
    {
        std::cout << "YOU WON !!!" << std::endl;
        char a; std::cin >> a;
        win = true;
    }
    return win;
}

