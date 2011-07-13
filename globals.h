#ifndef GLOBALS_H
#define GLOBALS_H

const int BOARD_WIDTH          = 8;                    // number of tiles along width of board
const int BOARD_HEIGHT         = BOARD_WIDTH;          // number of tiles along length of board

const int TILE_WIDTH    = 80;
const int TILE_HEIGHT   = 80;
const int TILE_SPRITES  = 12;

const int TILE_COUNT    = 8/*BOARD_WIDTH*/  * 8/*BOARD_HEIGHT*/;
const int SCREEN_WIDTH  = 8/*BOARD_WIDTH*/  * TILE_WIDTH;
const int SCREEN_HEIGHT = 8/*BOARD_HEIGHT*/ * TILE_HEIGHT;
const int SCREEN_BPP    = 32;

enum tiles_t
{
	TILE_NONE = -1,
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

#endif
