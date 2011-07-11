#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "Tile.h"

Tile::Tile( int x, int y, int tile_type )
{

    xcoord = x / TILE_WIDTH;
    ycoord = y / TILE_HEIGHT;

    set_type(tile_type);

}

void Tile::show(SDL_Surface* tileset, SDL_Surface* screen, SDL_Rect clips[])
{

	SDL_Rect offset;
	offset.x = xcoord * TILE_WIDTH;
	offset.y = ycoord * TILE_HEIGHT;

	SDL_BlitSurface( tileset, &clips[get_type()], screen, &offset );

}

bool Tile::equals(Tile* tile)
{
	return ( ( this->xcoord == tile->get_xcoord() )
		&& ( this->ycoord == tile->get_ycoord() ) );
}

void Tile::set_type( int type ) { this->type = type; }
int Tile::get_type() { return this->type; }

void Tile::set_xcoord( int xcoord ) { this->xcoord = xcoord; }
void Tile::set_ycoord( int ycoord ) { this->ycoord = ycoord; }

void Tile::set_coords( int xcoord, int ycoord )
{
	set_xcoord( xcoord );
	set_ycoord( ycoord );
}

int Tile::get_xcoord() { return this->xcoord; }
int Tile::get_ycoord() { return this->ycoord; }

void Tile::unset() { xcoord = -1; ycoord = -1; type = TILE_NONE; }
