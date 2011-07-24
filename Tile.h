#ifndef TILE_H
#define TILE_H

#include <SDL/SDL.h>
#include "Settings.h"
#include "Window.h"

enum tiles_t
{
	TILE_INVALID = -2, TILE_NONE = -1,
	TILE_BLACK, TILE_BLACK_SELECTED,
	TILE_WHITE, TILE_WHITE_SELECTED,
	TILE_RED,   TILE_RED_KING,   TILE_RED_SELECTED,   TILE_RED_POSSIBLE,
	TILE_GREEN, TILE_GREEN_KING, TILE_GREEN_SELECTED, TILE_GREEN_POSSIBLE
};

enum directions_t
{
	DIRECTION_NONE = -1,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

class Tile
{

    private:
        int type;
        int xcoord;
        int ycoord;
		Settings* settings;
		Window* window;

    public:
        Tile( int x, int y, int tile_type );
		void show();
        void set_type( int type );
        int get_type();
		void set_xcoord( int xcoord );
		void set_ycoord( int ycoord );
		void set_coords( int xcoord, int ycoord );
        int get_xcoord();
        int get_ycoord();
		bool equals( Tile* tile );
		void unset();

};

#endif
