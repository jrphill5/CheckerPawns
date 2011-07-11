#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"

class Board
{

	private:
		int width;
		int height;
		Tile* selected_tile;
		Tile* chosen_tile;
		Tile* board_tiles[ BOARD_WIDTH ][ BOARD_HEIGHT ];
		Tile* piece_tiles[ BOARD_WIDTH ][ BOARD_HEIGHT ];
		Tile* possible_moves[ BOARD_WIDTH ][ BOARD_HEIGHT ];

	public:
		Board(int width, int height);
		void clear_possible_moves();
		void reset();
		void clean();
		void show(SDL_Surface* tileset, SDL_Surface* screen, SDL_Rect clips[]);
		void capture_piece( Tile* &old_piece, Tile* &new_piece );
		int get_width();
		int get_height();
		Tile* get_board_tile(int x, int y);
		Tile* get_piece_tile(int x, int y);
		Tile* get_possible_moves(int x, int y);
		Tile* get_selected_tile();
		Tile* get_chosen_tile();
		void select_tile(int x, int y);
		void choose_tile(int x, int y);
		int get_tile_count(int type);

};

#endif
