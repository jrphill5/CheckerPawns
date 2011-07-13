#include "Settings.h"

Settings* Settings::pInstance = NULL;

Settings* Settings::CreateInstance()
{
	if (!pInstance) pInstance = new Settings();
	return pInstance;
}

Settings::Settings()
{

	parse_settings( read_file( "settings.txt" ) );

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
		cout << "  Line " << i+1 << ": ";
		vector<string> params = explode( settingsData[i], '#' ); // remove comments
		if ( params.size() != 0 )
		{
			vector<string> parsed = explode( params[0], '=' );    // parse parameters
			if ( parsed.size() == 2 )
			{
				string param = trim( parsed[0] );
				int value = atoi( trim( parsed[1] ).c_str() );
				settingsMap[ param ] = value;
				printf( "%-20s %d", param.c_str(), value );
			}
			else cout << "syntax error";
			cout << endl;
		} else cout << "empty line" << endl;
	}

}

void Settings::printSettings()
{

	for ( map<string, int>::iterator iter = settingsMap.begin() ; iter != settingsMap.end() ; ++iter )
		printf( "%-20s %d\n", iter->first.c_str(), iter->second );

}

const int Settings::retrieve( string param )
{
	
	return settingsMap[ param ];

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
