#ifndef ROW_H
#define ROW_H

#include "Settings.h"
#include "Tile.h"

class Row
{

	private:
		int ycoord;
		int size;
		int type;
		vector< Tile* > tiles;
		Settings* settings;
	public:
		Row( int ycoord, int size, int type );
		void clear();
		Tile* get_tile( int index );
		void show(SDL_Surface* tileset, SDL_Surface* screen, vector<SDL_Rect> clips);

};

#endif
