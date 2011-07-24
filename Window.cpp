#include "Window.h"

Window* Window::pInstance = NULL;

Window* Window::CreateInstance()
{
	if (!pInstance) pInstance = new Window();
	return pInstance;
}

Window::Window()
{

	settings = Settings::CreateInstance();
	screen = NULL; tileset = NULL;
	init_screen();

}

bool Window::init_screen()
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		cout << "SDL was unable to initialize!" << "\n";
		return false;
	}
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	cout << "Native Resolution: " << info->current_w << "x" << info->current_h << "\n";
	cout << "Window Resolution: " << settings->retrieve("SCREEN_WIDTH") << "x" << settings->retrieve("SCREEN_HEIGHT") << "\n";
	cout << "Bits Per Pixel:    " << settings->retrieve("SCREEN_BPP") << "\n";
	cout << "Hardware Surfaces: " << ( info->hw_available ? "yes" : "no" ) << "\n";
	cout << "Window Manager:    " << ( info->wm_available ? "yes" : "no" ) << "\n";
	if ( settings->retrieve("SCREEN_WIDTH") > info->current_w || settings->retrieve("SCREEN_HEIGHT") > info->current_h )
	{
		cout << "Window resolution larger than screen resolution!" << "\n";
		return false;
	}
    screen = SDL_SetVideoMode( settings->retrieve("SCREEN_WIDTH"), settings->retrieve("SCREEN_HEIGHT"), settings->retrieve("SCREEN_BPP"), ( info->hw_available ? SDL_HWSURFACE : SDL_SWSURFACE ) );
	if ( screen == NULL )
	{
		cout << "Unable to initialize main window!";
		return false;
	}
	if ( SDL_EnableKeyRepeat( settings->retrieve("KEY_REPEAT_DELAY"), settings->retrieve("KEY_REPEAT_INTERVAL") ) == -1 || settings->retrieve("KEY_REPEAT_DELAY") == 0 )
	{
		cout << "Key Repeat:        no" << "\n";
	}
	else
	{
		cout << "Key Repeat:        yes" << "\n";
		cout << " - Delay:          " << settings->retrieve("KEY_REPEAT_DELAY")    << "ms" << "\n";
		cout << " - Interval:       " << settings->retrieve("KEY_REPEAT_INTERVAL") << "ms" << "\n";
	}
	tileset = load_image( "tiles.png" );
	if ( tileset == NULL )
	{
		cout << "Unable to load sprites!" << "\n";
		return false;
	}
	if ( info->wm_available ) SDL_WM_SetCaption( "CheckerPawns", NULL );
    
	clip_tiles();

	return true;
}

SDL_Surface* Window::load_image( string filename )
{

    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );

    if ( loadedImage != NULL )
    {

        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );

        if ( optimizedImage != NULL )
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );

    }

    return optimizedImage;

}

void Window::clip_tiles()
{

	sprites.resize(settings->retrieve("TILE_SPRITES"));
	for ( int i = 0 ; i < settings->retrieve("TILE_SPRITES") ; i++ )
	{
		sprites[i].x = settings->retrieve("TILE_WIDTH") * ( i / 2 );
		sprites[i].y = ( ( i % 2 ) ? settings->retrieve("TILE_HEIGHT") : 0 );
		sprites[i].w = settings->retrieve("TILE_WIDTH");
		sprites[i].h = settings->retrieve("TILE_HEIGHT");
	}

}

bool Window::update()
{
	if ( SDL_Flip( get_screen() ) == -1 )
	{
		cout << "Failed to update the screen!" << endl;
		return false;
	}
	else return true;
}

void Window::clean()
{
	SDL_FreeSurface( screen );
	SDL_FreeSurface( tileset );
}

SDL_Surface* Window::get_screen()
{
	return screen;
}

SDL_Surface* Window::get_tileset()
{
	return tileset;
}

vector<SDL_Rect> Window::get_sprites()
{
	return sprites;
}
