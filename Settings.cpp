#include "Settings.h"

Settings::Settings( string filename )
{

	parse_settings( read_file( filename ) );

}

vector<string> Settings::read_file( string filename )
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
	}
	settingsFile.close();

	return settingsData;

}

void Settings::parse_settings( vector<string> settingsData )
{

	for ( vector<string>::size_type i = 0 ; i < settingsData.size() ; i++ )
	{
		cout << "  Line " << i << ": ";
		vector<string> params = explode( settingsData[i], '#' ); // remove comments
		if ( params.size() != 0 )
		{
			vector<string> parsed = explode( params[0], '=' );    // parse parameters
			for (vector<string>::size_type j = 0; j < parsed.size(); j++)
			{
				cout << "'" << trim(parsed[j]) << "' ";
			}
			cout << endl;
		}
	}

}

vector<string> &Settings::explode(const string &s, char delim, vector<string> &elems)
{
	stringstream ss(s);
	string item;
	while(getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

vector<string> Settings::explode(const string &s, char delim)
{
	vector<string> elems;
	return explode(s, delim, elems);
}

inline string &Settings::ltrim(string &s)
{
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	return s;
}

inline string &Settings::rtrim(string &s)
{
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}

inline string &Settings::trim(string &s)
{
	return ltrim(rtrim(s));
}
