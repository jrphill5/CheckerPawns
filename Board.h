#ifndef BOARD_H
#define BOARD_H

#include <SDL/SDL.h>
#include "Settings.h"
#include "Row.h"
#include "Tile.h"
#include <iostream>
#include <vector>

class Board
{

	private:
		int width;
		int height;
		Tile* selected_tile;
		Tile* chosen_tile;
		vector< Row* > board_tiles;
		vector< Row* > piece_tiles;
		vector< Row* > possible_moves;
		Settings* settings;

	public:
		Board(int width, int height);
		void clear_possible_moves();
		void reset();
		void clean();
		void show();
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
		bool check_winner();
		void move_cursor( int direction );
		void choose_tile();
		void set_possible_moves();
		void set_jump_moves( int xcoord, int ycoord, int type );

};

#endif
