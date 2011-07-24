#include "Row.h"

Row::Row( int ycoord, int size, int type )
{

	settings = Settings::CreateInstance();
	this->ycoord = ycoord;
	this->size = size;
	this->type = type;

	tiles.resize( size );

	for ( int i = 0 ; i < size ; i++ )
	{
		if ( type == TILE_BLACK )
			tiles[i] = new Tile( i, ycoord, ( i % 2 ) ? TILE_BLACK : TILE_WHITE );
		else if ( type == TILE_WHITE )
			tiles[i] = new Tile( i, ycoord, ( i % 2 ) ? TILE_WHITE : TILE_BLACK );
		else tiles[i] = new Tile( i, ycoord, type );
	}

}

void Row::clear()
{
	for ( int i = 0 ; i < size ; i++ )
		tiles[i]->set_type( TILE_NONE );
}

Tile* Row::get_tile( int index )
{
	return tiles[ index ];
}

void Row::show()
{
	for ( int i = 0 ; i < this->size ; i++ )
		tiles[i]->show();
}

