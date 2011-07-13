#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>

using namespace std;

class Settings
{

	private:
		int PIECE_ROWS;
		int BOARD_WIDTH;
		int BOARD_HEIGHT;
		int KEY_REPEAT_DELAY;
		int KEY_REPEAT_INTERVAL;
	public:
		Settings( string filename );
};

#endif
