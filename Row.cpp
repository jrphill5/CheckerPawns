#include <SDL/SDL.h>
#include "Row.h"

Row::Row( int size );
{

	settings = Settings::CreateInstance();
	this->size = size;

}
