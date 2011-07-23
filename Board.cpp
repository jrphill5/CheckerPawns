#include "Board.h"

Board::Board( int width, int height )
{

	settings = Settings::CreateInstance();

	this->width = width;
	this->height = height;

	board_tiles.resize( height );
	piece_tiles.resize( height );
	possible_moves.resize( height );

	this->reset();

}

void Board::reset()
{

	for ( int i = 0 ; i < this->height ; i++ )
	{

		int board_tile_type;
		int piece_tile_type;

		board_tile_type = (i%2) ? TILE_BLACK : TILE_WHITE;

		if ( i < settings->retrieve("PIECE_ROWS") ) piece_tile_type = TILE_GREEN;
		else if ( i > this->get_height()-settings->retrieve("PIECE_ROWS")-1 ) piece_tile_type = TILE_RED;
		else piece_tile_type = TILE_NONE;

		board_tiles[i]    = new Row( i, width, board_tile_type );
		piece_tiles[i]    = new Row( i, width, piece_tile_type );
		possible_moves[i] = new Row( i, width, TILE_NONE );

	}

	select_tile(0,0);
	choose_tile(-1,-1);

}

void Board::clear_possible_moves()
{
	for ( int i = 0 ; i < this->height ; i++ )
		possible_moves[i]->clear();
}

void Board::clean()
{
	for ( int i = 0 ; i < this->width ; i++ )
	{
		delete board_tiles[i];
		delete piece_tiles[i];
		delete possible_moves[i];
	}
	delete selected_tile;
	delete chosen_tile;
}

void Board::show(SDL_Surface* tileset, SDL_Surface* screen, vector<SDL_Rect> clips)
{
	for ( int i = 0 ; i < this->height ; i++ )
	{
		board_tiles[i]->show(tileset, screen, clips);
		piece_tiles[i]->show(tileset, screen, clips);
		possible_moves[i]->show(tileset, screen, clips);
		selected_tile->show(tileset, screen, clips);
		chosen_tile->show(tileset, screen, clips);
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

			if ( captured_xindex < 0 || captured_yindex < 0 ) continue;
			if ( captured_xindex > settings->retrieve("BOARD_WIDTH")-1 ||
				captured_yindex > settings->retrieve("BOARD_HEIGHT")-1 ) continue;

            int captured_type = get_piece_tile(captured_xindex, captured_yindex)->get_type();

            if ( ( delta_xcoord == x ) && ( delta_ycoord == y) )
            {

                if ( old_type == TILE_RED || old_type == TILE_RED_KING )
                    if ( ( captured_type != TILE_RED ) && ( captured_type != TILE_RED_KING ) )
                        if ( j == -1 || old_type == TILE_RED_KING )
                            get_piece_tile(captured_xindex, captured_yindex)->set_type( TILE_NONE );

                if ( old_type == TILE_GREEN || old_type == TILE_GREEN_KING )
                    if ( ( captured_type != TILE_GREEN ) && ( captured_type != TILE_GREEN_KING ) )
                        if ( j == 1 || old_type == TILE_GREEN_KING )
                            get_piece_tile(captured_xindex, captured_yindex)->set_type( TILE_NONE );

            }

        }

    }

}

Tile* Board::get_board_tile(int x, int y)
{
	if ( ( x >= 0 && x < width ) && ( y >= 0 && y < height ) )
		return board_tiles[y]->get_tile(x);
	else return NULL;
}

Tile* Board::get_piece_tile(int x, int y)
{
	if ( ( x >= 0 && x < width ) && ( y >= 0 && y < height ) )
		return piece_tiles[y]->get_tile(x);
	else return NULL;
}

Tile* Board::get_possible_moves(int x, int y)
{
	if ( ( x >= 0 && x < width ) && ( y >= 0 && y < height ) )
		return possible_moves[y]->get_tile(x);
	else return NULL;
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
			if ( this->get_piece_tile(i,j)->get_type() == type ) count ++;

	return count;

}

bool Board::check_winner()
{

	if ( get_tile_count( TILE_RED ) + get_tile_count( TILE_RED_KING ) == 0 )
	{ cout << "Green Wins!" << "\n"; return true; }

	if ( get_tile_count( TILE_GREEN ) + get_tile_count( TILE_GREEN_KING ) == 0 )
	{ cout << "Red Wins!" << "\n"; return true; }

	return false;

}

void Board::set_jump_moves( int color, int captured_xindex, int captured_yindex, int move_xindex, int move_yindex )
{

	int old_type = get_piece_tile( captured_xindex, captured_yindex )->get_type();
	int new_type = get_piece_tile( move_xindex, move_yindex )->get_type();
	
	if ( ( move_xindex != -1 && move_yindex != -1 ) &&
	   ( ( old_type == ( ( color == TILE_RED ) ? TILE_GREEN : TILE_RED ) ) ||
		 ( old_type == ( ( color == TILE_RED ) ? TILE_GREEN_KING : TILE_RED_KING ) ) ) &&
		 ( new_type == TILE_NONE ) )
		get_possible_moves(move_xindex,move_yindex)->set_type( ( color == TILE_RED ) ? TILE_RED_POSSIBLE : TILE_GREEN_POSSIBLE );

}

void Board::move_cursor( int direction )
{

    get_selected_tile()->set_type( TILE_NONE );

    int xcoord = get_selected_tile()->get_xcoord();
    int ycoord = get_selected_tile()->get_ycoord();

    switch ( direction )
    {

        case DIRECTION_UP:    if ( ycoord > 0 )					ycoord--; break;
        case DIRECTION_DOWN:  if ( ycoord < get_height()-1 )	ycoord++; break;
        case DIRECTION_LEFT:  if ( xcoord > 0 )					xcoord--; break;
        case DIRECTION_RIGHT: if ( xcoord < get_width()-1 )		xcoord++; break;

    }

	get_selected_tile()->set_coords( xcoord, ycoord );

	Tile* selected_board = get_board_tile( xcoord, ycoord );

    if ( selected_board->get_type() == TILE_BLACK )
        get_selected_tile()->set_type( TILE_BLACK_SELECTED );
    else if ( selected_board->get_type() == TILE_WHITE )
        get_selected_tile()->set_type( TILE_WHITE_SELECTED );

}

void Board::choose_tile()
{

    int old_type;
    int new_type;

	Tile* old_chosen_piece = get_piece_tile( get_chosen_tile()->get_xcoord(), get_chosen_tile()->get_ycoord() );
	Tile* new_chosen_piece = get_piece_tile( get_selected_tile()->get_xcoord(), get_selected_tile()->get_ycoord() );

    if ( old_chosen_piece ) old_type = old_chosen_piece->get_type();
    if ( new_chosen_piece ) new_type = new_chosen_piece->get_type();

    get_chosen_tile()->set_coords( new_chosen_piece->get_xcoord(), new_chosen_piece->get_ycoord() );

	if ( new_type != TILE_NONE )
	{

		clear_possible_moves();
        if ( old_chosen_piece && old_chosen_piece->equals( new_chosen_piece ) )
            get_chosen_tile()->unset();
        else
            set_possible_moves( new_chosen_piece );

	}
	else
	{

		if ( get_possible_moves( new_chosen_piece->get_xcoord(), new_chosen_piece->get_ycoord() )->get_type() != TILE_NONE )
		{

			if ( old_chosen_piece )
			{
		
				clear_possible_moves();

				new_chosen_piece->set_type( old_type );

				capture_piece( old_chosen_piece, new_chosen_piece );

				if ( old_type == TILE_RED && get_chosen_tile()->get_ycoord() == 0 )
					new_chosen_piece->set_type( TILE_RED_KING );

				if ( old_type == TILE_GREEN && get_chosen_tile()->get_ycoord() == get_height()-1 )
					new_chosen_piece->set_type( TILE_GREEN_KING );

				old_chosen_piece->set_type( TILE_NONE );

			}

		}
		else clear_possible_moves();

		get_chosen_tile()->unset();

	}

}

void Board::set_possible_moves( Tile* &chosen_piece )
{

    int type   = chosen_piece->get_type();
	int width  = get_width();
	int height = get_height();
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

        get_chosen_tile()->set_type( TILE_RED_SELECTED );

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
					set_jump_moves( TILE_RED, normal_move_x, normal_move_y, jump_move_x, jump_move_y );
				if ( get_piece_tile(      normal_move_x, normal_move_y )->get_type() == TILE_NONE )
					get_possible_moves(  normal_move_x, normal_move_y )->set_type( TILE_RED_POSSIBLE );

			}

        }

    }
    else if ( ( type == TILE_GREEN ) || ( type == TILE_GREEN_KING ) )
    {

        get_chosen_tile()->set_type( TILE_GREEN_SELECTED );

		for ( int i = 3 ; i >= 0 ; i-- )
		{

			int normal_move_x = movement[i][0][0];
			int normal_move_y = movement[i][0][1];
			int jump_move_x   = movement[i][1][0];
			int jump_move_y   = movement[i][1][1];

			if ( normal_move_x != -1 && normal_move_y != -1 )
			{

				if ( jump_move_x != -1 && jump_move_y != -1 )
					set_jump_moves( TILE_GREEN, normal_move_x, normal_move_y, jump_move_x, jump_move_y );
				if ( get_piece_tile(        normal_move_x, normal_move_y )->get_type() == TILE_NONE )
					get_possible_moves(    normal_move_x, normal_move_y )->set_type( TILE_GREEN_POSSIBLE );

			}
			
			if ( ( chosen_piece->get_type() == TILE_GREEN ) && ( i == 2 ) ) break;

        }

    }

}
