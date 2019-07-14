/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  <copyright holder> <email>
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
 *
 */

#ifndef VIEW2D_H
#define VIEW2D_H

#include "sdl_dep.h"
#include "board.h"

class View2D
{
    static bool SDL_Initialized;
    static void init();
    
    int SCR_WIDTH;
    int SCR_HEIGHT;
    const char * _winName;
    SDL_Window * _win = nullptr;
    SDL_Renderer * _render = nullptr;
    SDL_Texture * _stone[2] = {};
    SDL_Texture * _boardTexture = nullptr;
    SDL_Texture * _boardBackground;
    SDL_Rect _boardPosition;
    
    Board & _board;
  
    void createWindow();
    void loadTextureImage();
    
    void showStones() const;
    void show() const;
    
public:
    View2D(const char* name,const int w, const int h, Board& );
    ~View2D(); 
    void select();
};

#endif // VIEW2D_H
