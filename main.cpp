#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <string>
#include "Board.h"
#include "Settings.h"

using namespace std;

bool init_screen();
SDL_Surface* load_image( string filename );
void clip_tiles();

Settings* settings = Settings::CreateInstance();
SDL_Surface *screen  = NULL;
SDL_Surface *tileset = NULL;
vector<SDL_Rect> sprites;
SDL_Event event;

int main ( int argc, char* args[] )
{

	if ( !init_screen() ) return 1;

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
        board->show(tileset, screen, sprites);
        board->get_selected_tile()->show(tileset, screen, sprites);
        if ( SDL_Flip( screen ) == -1 )
		{ cout << "Failed to update the screen!" << "\n"; return 2; }
		if ( board->check_winner() ) return 0;
    }

    SDL_FreeSurface( tileset );
    SDL_FreeSurface( screen );

	board->clean();

    SDL_Quit();

	return 0;

}

bool init_screen()
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		cout << "SDL was unable to initialize!" << "\n";
		return false;
	}
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	cout << "Native Resolution: " << info->current_w << "x" << info->current_h << "\n";
	cout << "Window Resolution: " << settings->retrieve("SCREEN_WIDTH") << "x" << settings->retrieve("SCREEN_HEIGHT") << "\n";
	cout << "Bits Per Pixel:    " << settings->retrieve("SCREEN_BPP") << "\n";
	cout << "Hardware Surfaces: " << ( info->hw_available ? "yes" : "no" ) << "\n";
	cout << "Window Manager:    " << ( info->wm_available ? "yes" : "no" ) << "\n";
	if ( settings->retrieve("SCREEN_WIDTH") > info->current_w || settings->retrieve("SCREEN_HEIGHT") > info->current_h )
	{
		cout << "Window resolution larger than screen resolution!" << "\n";
		return false;
	}
    screen = SDL_SetVideoMode( settings->retrieve("SCREEN_WIDTH"), settings->retrieve("SCREEN_HEIGHT"), settings->retrieve("SCREEN_BPP"), ( info->hw_available ? SDL_HWSURFACE : SDL_SWSURFACE ) );
	if ( screen == NULL )
	{
		cout << "Unable to initialize main window!";
		return false;
	}
	if ( SDL_EnableKeyRepeat( settings->retrieve("KEY_REPEAT_DELAY"), settings->retrieve("KEY_REPEAT_INTERVAL") ) == -1 || settings->retrieve("KEY_REPEAT_DELAY") == 0 )
	{
		cout << "Key Repeat:        no" << "\n";
	}
	else
	{
		cout << "Key Repeat:        yes" << "\n";
		cout << " - Delay:          " << settings->retrieve("KEY_REPEAT_DELAY")    << "ms" << "\n";
		cout << " - Interval:       " << settings->retrieve("KEY_REPEAT_INTERVAL") << "ms" << "\n";
	}
	tileset = load_image( "tiles.png" );
	if ( tileset == NULL )
	{
		cout << "Unable to load sprites!" << "\n";
		return false;
	}
	if ( info->wm_available ) SDL_WM_SetCaption( "CheckerPawns", NULL );
    
	clip_tiles();

	return true;
}

SDL_Surface* load_image( string filename )
{

    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );

    if ( loadedImage != NULL )
    {

        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );

        if ( optimizedImage != NULL )
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );

    }

    return optimizedImage;

}

void clip_tiles()
{

	sprites.resize(settings->retrieve("TILE_SPRITES"));
	for ( int i = 0 ; i < settings->retrieve("TILE_SPRITES") ; i++ )
	{
		sprites[i].x = settings->retrieve("TILE_WIDTH") * ( i / 2 );
		sprites[i].y = ( ( i % 2 ) ? settings->retrieve("TILE_HEIGHT") : 0 );
		sprites[i].w = settings->retrieve("TILE_WIDTH");
		sprites[i].h = settings->retrieve("TILE_HEIGHT");
	}

}
