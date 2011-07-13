#include <SDL/SDL.h>
#include "Row.h"

Row::Row();
{

	settings = Settings::CreateInstance();
	this->size = settings->retreive("BOARD_WIDTH");

}
