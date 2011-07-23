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
void choose_tile( Board* board );
void set_possible_moves( Board* board, Tile* &chosen_piece );

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
                    case SDLK_RETURN: choose_tile( board );        break;
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


void choose_tile( Board* board )
{

    int old_type;
    int new_type;

	cout << "lol" << endl;
	int xcoord = board->get_chosen_tile()->get_xcoord();
	cout << xcoord << endl;
	int ycoord = board->get_chosen_tile()->get_ycoord();
	cout << ycoord << endl;
	Tile* old_chosen_piece = board->get_piece_tile( xcoord, ycoord );
	cout << "lol" << endl;
	Tile* new_chosen_piece = board->get_piece_tile( board->get_selected_tile()->get_xcoord(), board->get_selected_tile()->get_ycoord() );
	cout << "lol" << endl;

    if ( old_chosen_piece != NULL ) old_type = old_chosen_piece->get_type();
    if ( new_chosen_piece != NULL ) new_type = new_chosen_piece->get_type();

    board->get_chosen_tile()->set_coords( new_chosen_piece->get_xcoord(), new_chosen_piece->get_ycoord() );

	if ( new_type != TILE_NONE )
	{

		board->clear_possible_moves();
        if ( old_chosen_piece != NULL && old_chosen_piece->equals( new_chosen_piece ) )
            board->get_chosen_tile()->unset();
        else
            set_possible_moves( board, new_chosen_piece );

	}
	else
	{

		if ( board->get_possible_moves( new_chosen_piece->get_xcoord(), new_chosen_piece->get_ycoord() )->get_type() != TILE_NONE )
		{

			if ( old_chosen_piece != NULL )
			{
		
				board->clear_possible_moves();

				new_chosen_piece->set_type( old_type );

				board->capture_piece( old_chosen_piece, new_chosen_piece );

				if ( old_type == TILE_RED && board->get_chosen_tile()->get_ycoord() == 0 )
					new_chosen_piece->set_type( TILE_RED_KING );

				if ( old_type == TILE_GREEN && board->get_chosen_tile()->get_ycoord() == board->get_height()-1 )
					new_chosen_piece->set_type( TILE_GREEN_KING );

				old_chosen_piece->set_type( TILE_NONE );

			}

		}
		else board->clear_possible_moves();

		board->get_chosen_tile()->unset();

	}

}

void set_possible_moves( Board* board, Tile* &chosen_piece )
{

    int type   = chosen_piece->get_type();
	int width  = board->get_width();
	int height = board->get_height();
    int xcoord = chosen_piece->get_xcoord();
    int ycoord = chosen_piece->get_ycoord();
	int movement[4][2][2];

	// normal moves
	if ( xcoord > 0 && ycoord > 0 )					{ movement[0][0][0] = xcoord-1; /*left*/	movement[0][0][1] = ycoord-1; /*up*/ }
	else											{ movement[0][0][0] = -1;					movement[0][0][1] = -1; }
	if ( xcoord < width-1 && ycoord > 0 )			{ movement[1][0][0] = xcoord+1; /*right*/	movement[1][0][1] = ycoord-1; /*up*/ }
	else											{ movement[1][0][0] = -1;					movement[1][0][1] = -1; }
	if ( xcoord > 0 && ycoord < height-1 )			{ movement[2][0][0] = xcoord-1; /*left*/	movement[2][0][1] = ycoord+1; /*down*/ }
	else											{ movement[2][0][0] = -1;					movement[2][0][1] = -1; }
	if ( xcoord < width-1 && ycoord < height-1 )	{ movement[3][0][0] = xcoord+1; /*right*/	movement[3][0][1] = ycoord+1; /*down*/ }
	else											{ movement[3][0][0] = -1;					movement[3][0][1] = -1; }

	// jump moves
	if ( xcoord > 1 && ycoord > 1 )					{ movement[0][1][0] = xcoord-2; /*left*/	movement[0][1][1] = ycoord-2; /*up*/ }
	else											{ movement[0][1][0] = -1;					movement[0][1][1] = -1; }
	if ( xcoord < width-2 && ycoord > 1 )			{ movement[1][1][0] = xcoord+2; /*right*/	movement[1][1][1] = ycoord-2; /*up*/ }
	else											{ movement[1][1][0] = -1;					movement[1][1][1] = -1; }
	if ( xcoord > 1 && ycoord < height-2 )			{ movement[2][1][0] = xcoord-2; /*left*/	movement[2][1][1] = ycoord+2; /*down*/ }
	else											{ movement[2][1][0] = -1;					movement[2][1][1] = -1; }
	if ( xcoord < width-2 && ycoord < height-2 )	{ movement[3][1][0] = xcoord+2; /*right*/	movement[3][1][1] = ycoord+2; /*down*/ }
	else											{ movement[3][1][0] = -1;					movement[3][1][1] = -1; }

    if ( ( type == TILE_RED ) || ( type == TILE_RED_KING ) )
    {

        board->get_chosen_tile()->set_type( TILE_RED_SELECTED );

		for ( int i = 0 ; i <= 3 ; i++ )
		{

			int normal_move_x = movement[i][0][0];
			int normal_move_y = movement[i][0][1];
			int jump_move_x   = movement[i][1][0];
			int jump_move_y   = movement[i][1][1];

			if ( ( chosen_piece->get_type() == TILE_RED ) && ( i == 2 ) ) break;

			if ( normal_move_x != -1 && normal_move_y != -1 )
			{

				if ( jump_move_x != -1 && jump_move_y != -1 )
					board->set_jump_moves( TILE_RED, normal_move_x, normal_move_y, jump_move_x, jump_move_y );
				if ( board->get_piece_tile(      normal_move_x, normal_move_y )->get_type() == TILE_NONE )
					board->get_possible_moves(  normal_move_x, normal_move_y )->set_type( TILE_RED_POSSIBLE );

			}

        }

    }
    else if ( ( type == TILE_GREEN ) || ( type == TILE_GREEN_KING ) )
    {

        board->get_chosen_tile()->set_type( TILE_GREEN_SELECTED );

		for ( int i = 3 ; i >= 0 ; i-- )
		{

			int normal_move_x = movement[i][0][0];
			int normal_move_y = movement[i][0][1];
			int jump_move_x   = movement[i][1][0];
			int jump_move_y   = movement[i][1][1];

			if ( normal_move_x != -1 && normal_move_y != -1 )
			{

				if ( jump_move_x != -1 && jump_move_y != -1 )
					board->set_jump_moves( TILE_GREEN, normal_move_x, normal_move_y, jump_move_x, jump_move_y );
				if ( board->get_piece_tile(        normal_move_x, normal_move_y )->get_type() == TILE_NONE )
					board->get_possible_moves(    normal_move_x, normal_move_y )->set_type( TILE_GREEN_POSSIBLE );

			}
			
			if ( ( chosen_piece->get_type() == TILE_GREEN ) && ( i == 2 ) ) break;

        }

    }

}
