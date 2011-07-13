#ifndef TILE_H
#define TILE_H

#include "Settings.h"
#include "globals.h"

class Tile
{

    private:
        int type;
        int xcoord;
        int ycoord;
		Settings* settings;

    public:
        Tile( int x, int y, int tile_type );
		void show(SDL_Surface* tileset, SDL_Surface* screen, SDL_Rect clips[]);
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
