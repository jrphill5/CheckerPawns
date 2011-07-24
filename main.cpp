#include <SDL/SDL.h>
#include "Board.h"
#include "Settings.h"
#include "Window.h"

using namespace std;

Settings* settings = Settings::CreateInstance();
Window* window = Window::CreateInstance();

int main ( int argc, char* args[] )
{

	Board* board = new Board( settings->retrieve("BOARD_WIDTH"), settings->retrieve("BOARD_HEIGHT") );

    bool quit = false;
    while ( !quit )
    {

		SDL_Event event;
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

		board->move_cursor( direction );
        board->show();
        if ( !window->update() ) return 1;
		if ( board->check_winner() ) return 0;
    }

	board->clean();
    window->clean();

    SDL_Quit();

	return 0;

}

