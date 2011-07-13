#include "Settings.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <locale> 

using namespace std;

static inline string &ltrim(string &s) {
s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
return s;
}

static inline string &rtrim(string &s) {
s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
return s;
}

static inline string &trim(string &s) {
return ltrim(rtrim(s));
}

vector<string> &split(const string &s, char delim, vector<string> &elems)
{
	stringstream ss(s);
	string item;
	while(getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

vector<string> split(const string &s, char delim)
{
	vector<string> elems;
	return split(s, delim, elems);
}

Settings::Settings( string filename )
{

	ifstream settingsFile;
	vector<string> settingsData;
	settingsFile.open( filename.c_str() );
	if ( settingsFile.is_open() )
	{
		cout << "Reading " << filename << ":" << endl;
		string settingsLine;
		while ( getline( settingsFile, settingsLine ) )
			settingsData.push_back( settingsLine );
	}
	else
	{
		cout << filename << " could not be read!" << endl;
		// set default settings ...
		return;
	}
	settingsFile.close();

	for ( vector<string>::size_type i = 0 ; i < settingsData.size() ; i++ )
	{
		cout << "  Line " << i << ": ";
		vector<string> params = split( settingsData[i], '#' ); // remove comments
		if ( params.size() != 0 )
		{
			vector<string> parsed = split( params[0], '=' );    // parse parameters
			for (vector<string>::size_type j = 0; j < parsed.size(); j++)
			{
				cout << "'" << trim(parsed[j]) << "' ";
			}
			cout << endl;
		}
	}

}
