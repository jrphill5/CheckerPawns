#include <SDL/SDL.h>
#include "Board.h"

Board::Board( int width, int height )
{

	settings = Settings::CreateInstance();

	this->width = width;
	this->height = height;

	this->reset();

}

void Board::reset()
{

	for ( int i = 0 ; i < this->width ; i++ )
	{

		for ( int j = 0 ; j < this->height ; j++ )
		{

            int board_tile_type;
            int piece_tile_type;

            board_tile_type = ( i % 2 )
				? ( ( j % 2 ) ? TILE_BLACK : TILE_WHITE )
				: ( ( j % 2 ) ? TILE_WHITE : TILE_BLACK );

            if ( j < settings->retrieve("PIECE_ROWS") ) piece_tile_type = TILE_GREEN;
            else if ( j > this->get_height()-settings->retrieve("PIECE_ROWS")-1 ) piece_tile_type = TILE_RED;
            else piece_tile_type = TILE_NONE;

            board_tiles[i][j]    = new Tile( i, j, board_tile_type );
            piece_tiles[i][j]    = new Tile( i, j, piece_tile_type );
            possible_moves[i][j] = new Tile( i, j, TILE_NONE );

        }

    }

	select_tile(0,0);
	choose_tile(-1,-1);

}

void Board::clear_possible_moves()
{
	for ( int i = 0 ; i < this->width ; i ++ )
	{
		for ( int j = 0 ; j < this->height ; j ++ )
		{
			possible_moves[i][j]->set_type( TILE_NONE );
		}
	}
}

void Board::clean()
{
	for ( int i = 0 ; i < this->width ; i++ )
	{
		for ( int j = 0 ; j < this->width ; j++ )
		{
			delete board_tiles[i][j];
			delete piece_tiles[i][j];
			delete possible_moves[i][j];
		}
	}
	delete selected_tile;
	delete chosen_tile;
}

void Board::show(SDL_Surface* tileset, SDL_Surface* screen, SDL_Rect clips[])
{
	for ( int i = 0 ; i < this->width ; i++ )
	{
		for ( int j = 0 ; j < this->height ; j++ )
		{
			board_tiles[i][j]->show(tileset, screen, clips);
			piece_tiles[i][j]->show(tileset, screen, clips);
			possible_moves[i][j]->show(tileset, screen, clips);
			selected_tile->show(tileset, screen, clips);
			chosen_tile->show(tileset, screen, clips);
		}
	}
}

void Board::capture_piece( Tile* &old_piece, Tile* &new_piece )
{

    int old_type = old_piece->get_type();

    int old_xcoord = old_piece->get_xcoord();
    int old_ycoord = old_piece->get_ycoord();

    int new_xcoord = new_piece->get_xcoord();
    int new_ycoord = new_piece->get_ycoord();

    int delta_xcoord = old_xcoord - new_xcoord;
    int delta_ycoord = old_ycoord - new_ycoord;

    for ( int j = -1 ; j < 2 ; j+=2 )
    {

        for ( int i = -1 ; i < 2 ; i+= 2 )
        {

            int x = ( i < 0 ) ? i + 3 : i - 3;
            int y = ( j < 0 ) ? j + 3 : j - 3;

            int captured_yindex = old_ycoord + j;
			int captured_xindex = old_xcoord + i;
            int captured_type = piece_tiles[captured_xindex][captured_yindex]->get_type();

            if ( ( delta_xcoord == x ) && ( delta_ycoord == y) )
            {

                if ( old_type == TILE_RED || old_type == TILE_RED_KING )
                    if ( ( captured_type != TILE_RED ) && ( captured_type != TILE_RED_KING ) )
                        if ( j == -1 || old_type == TILE_RED_KING )
                            piece_tiles[captured_xindex][captured_yindex]->set_type( TILE_NONE );

                if ( old_type == TILE_GREEN || old_type == TILE_GREEN_KING )
                    if ( ( captured_type != TILE_GREEN ) && ( captured_type != TILE_GREEN_KING ) )
                        if ( j == 1 || old_type == TILE_GREEN_KING )
                            piece_tiles[captured_xindex][captured_yindex]->set_type( TILE_NONE );

            }

        }

    }

}

Tile* Board::get_board_tile(int x, int y)
{
	return board_tiles[x][y];
}

Tile* Board::get_piece_tile(int x, int y)
{
	return piece_tiles[x][y];
}

Tile* Board::get_possible_moves(int x, int y)
{
	return possible_moves[x][y];
}

Tile* Board::get_selected_tile()
{
	return selected_tile;
}

Tile* Board::get_chosen_tile()
{
	return chosen_tile;
}

int Board::get_width()
{
	return this->width;
}

int Board::get_height()
{
	return this->height;
}

void Board::select_tile(int x, int y)
{

	switch ( get_board_tile( x, y )->get_type() )
	{
		case TILE_BLACK:
			selected_tile = new Tile( x, y, TILE_BLACK_SELECTED );
			break;
		case TILE_WHITE:
			selected_tile = new Tile( x, y, TILE_WHITE_SELECTED );
			break;
	}

}

void Board::choose_tile(int x, int y)
{
    chosen_tile = new Tile( x, y, TILE_NONE );
}

int Board::get_tile_count(int type)
{

	int count = 0;

	for ( int i = 0 ; i < this->width ; i++ )
		for ( int j = 0 ; j < this->height ; j++ )
			if ( this->piece_tiles[i][j]->get_type() == type ) count ++;

	return count;

}
