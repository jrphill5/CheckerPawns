#ifndef ROW_H
#define ROW_H

#include <SDL/SDL.h>
#include "Settings.h"
#include "Tile.h"
#include <vector>

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
		void show();

};

#endif
