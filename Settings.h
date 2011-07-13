#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <locale> 

using namespace std;

class Settings
{

	private:
		int PIECE_ROWS;
		int BOARD_WIDTH;
		int BOARD_HEIGHT;
		int KEY_REPEAT_DELAY;
		int KEY_REPEAT_INTERVAL;
		vector<string> read_file( string filename );
		void parse_settings( vector<string> settingsData );
		vector<string> &explode(const string &s, char delim, vector<string> &elems);
		vector<string> explode(const string &s, char delim);
		inline string &ltrim(string &s);
		inline string &rtrim(string &s);
		inline string &trim(string &s);
	public:
		Settings( string filename );
		
};

#endif
