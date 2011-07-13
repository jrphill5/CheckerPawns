#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <locale> 

using namespace std;

class Settings
{

	private:
		Settings();
		Settings(Settings const&){};
		Settings& operator=(Settings const&){};
		static Settings* pInstance;
		map<string, int> settingsMap;
		vector<string> read_file( string filename );
		void parse_settings( vector<string> settingsData );
		vector<string> &explode(const string &s, char delim, vector<string> &elems);
		vector<string> explode(const string &s, char delim);
		inline string &ltrim(string &s);
		inline string &rtrim(string &s);
		inline string &trim(string &s);
	public:
		const int retrieve( string param );
		static Settings* CreateInstance();
		
};

#endif
