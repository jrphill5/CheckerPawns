#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <string>
#include "Board.h"
#include "Settings.h"
#include "Window.h"

using namespace std;

Settings* settings = Settings::CreateInstance();
Window* window = Window::CreateInstance();
SDL_Event event;

int main ( int argc, char* args[] )
{

    bool quit = false;

	Board* board = new Board(        settings->retrieve("BOARD_WIDTH"),          settings->retrieve("BOARD_HEIGHT"));
	cout << "Board Size:        " << settings->retrieve("BOARD_WIDTH") << "x" << settings->retrieve("BOARD_HEIGHT") << "\n";
	cout << "Player Rows:       " << settings->retrieve("PIECE_ROWS") << "\n";

    while ( !quit )
    {
		int direction = DIRECTION_NONE;
        while ( SDL_PollEvent( &event ) )
        {
            if ( event.type == SDL_QUIT ) quit = true;
            if ( event.type == SDL_KEYDOWN )
            {
                switch( event.key.keysym.sym )
                {
                    case SDLK_UP:     direction = DIRECTION_UP;    break;
                    case SDLK_DOWN:   direction = DIRECTION_DOWN;  break;
                    case SDLK_LEFT:   direction = DIRECTION_LEFT;  break;
                    case SDLK_RIGHT:  direction = DIRECTION_RIGHT; break;
                    case SDLK_RETURN: board->choose_tile();        break;
                    case SDLK_ESCAPE:
                    case SDLK_q:      quit = true;                 break;
                    case SDLK_r:      board->reset();              break;
                }
            }
        }
		if ( direction != DIRECTION_NONE ) board->move_cursor( direction );
        board->show(window->get_tileset(), window->get_screen(), window->get_sprites());
        board->get_selected_tile()->show(window->get_tileset(), window->get_screen(), window->get_sprites());
        if ( SDL_Flip( window->get_screen() ) == -1 )
		{ cout << "Failed to update the screen!" << "\n"; return 2; }
		if ( board->check_winner() ) return 0;
    }

    SDL_FreeSurface( window->get_tileset() );
    SDL_FreeSurface( window->get_screen() );

	board->clean();

    SDL_Quit();

	return 0;

}

