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

#include "view2d.h"

static const int STONE_SIZE = 60;
static const int OFFSET_X = 19;
static const int OFFSET_Y = 21;
static const int STEP_X = 88;
static const int STEP_Y = 88;

bool View2D::SDL_Initialized = false;

View2D::View2D(const char* name,const int w, const int h, Board& B)
:SCR_WIDTH(w)
,SCR_HEIGHT(h)
,_winName (name)
,_board(B)
{
    if(!SDL_Initialized )
    {
        init();
    } 
    createWindow(); 
}

void View2D::init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_Initialized = true;
}

void View2D::createWindow()
{
    _win = SDL_CreateWindow( _winName,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED, 
                           SCR_WIDTH,SCR_HEIGHT,SDL_WINDOW_SHOWN);
    if( _win == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() <<std::endl;
        exit(2);
    }
    _render = SDL_CreateRenderer( _win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(_render == nullptr)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        exit(3);
    } 
    loadTextureImage();
}

void View2D::loadTextureImage()
{
    int w,h;
    _boardBackground = IMG_LoadTexture(_render, "graphics/background.png");
    if( _boardBackground == nullptr)
    {
        std::cerr << "IMG_LoadTexture Error: "<< SDL_GetError() << std::endl;
        exit(5);
    }
    _boardTexture  = IMG_LoadTexture(_render, "graphics/board.png");
    if( _boardTexture == nullptr)
    {
        std::cerr << "IMG_LoadTexture Error: "<< SDL_GetError() << std::endl;
        exit(5);
    }
    _stone[0]  = IMG_LoadTexture(_render, "graphics/stone0.png");
    if( _boardTexture == nullptr)
    {
        std::cerr << "IMG_LoadTexture Error: "<< SDL_GetError() << std::endl;
        exit(5);
    }
    _stone[1]  = IMG_LoadTexture(_render, "graphics/stone1.png");
    if( _boardTexture == nullptr)
    {
        std::cerr << "IMG_LoadTexture Error: "<< SDL_GetError() << std::endl;
        exit(5);
    }
    SDL_QueryTexture( _boardTexture,NULL,NULL, &w,&h);
    
    _boardPosition.x=(SCR_WIDTH-w)/2;
    _boardPosition.y=(SCR_HEIGHT-h)/2;
    _boardPosition.w=w;
    _boardPosition.h=h;
}

void View2D::showStones() const
{
    for(int team = 0; team < 2; ++team)
        for(int ID = 1; ID < 6; ++ID)
        {
            SDL_Rect pos;
            const Node place = *_board.getStone(team == 0 ? - ID : ID);
            pos.x = OFFSET_X + (place.col - 1) * STEP_X;
            pos.y = OFFSET_Y + (place.row - 1) * STEP_Y;
            pos.w = STONE_SIZE;
            pos.h = STONE_SIZE;
            pos.y = SCR_HEIGHT - pos.y - STONE_SIZE; // vertical reflection
            SDL_RenderCopy(_render, _stone[team], NULL, &pos);
        }
}

void View2D::show() const
{
    SDL_RenderClear(_render);
    SDL_RenderCopy(_render, _boardBackground, NULL,NULL);
    SDL_RenderCopy(_render,_boardTexture,NULL,&_boardPosition );
    showStones();
    SDL_RenderPresent(_render);
}
void View2D::select()
{
    SDL_Event event;
    do
    {
        show();
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
        {
            return;
        }
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym) // ToDo
            {
                case SDLK_LEFT:
                    return;
                case SDLK_RIGHT:
                    if (_board.isFinished())
                    {
                        _board.reset();
                    }
                    else
                    {
                        _board.storeStep(_board.getStep());
                        _board.seekerSwap();
                    }             
                    break;
                case SDLK_ESCAPE:
                    return;
                default:
                    ;
            }
        }
    } while (true);
}

View2D::~View2D()
{
    SDL_DestroyTexture( _boardBackground ); std::cout << _winName <<" board background deleted " << std::endl;
    SDL_DestroyTexture( _boardTexture );    std::cout << _winName <<" board texture deleted " << std::endl;
    SDL_DestroyRenderer(_render);          std::cout << _winName <<" render deleted " << std::endl;
    SDL_DestroyWindow( _win );               std::cout << _winName <<" window closed " << std::endl;
    std::cout << std::endl << _winName <<" destructor finished... " << std::endl;
}
