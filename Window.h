#ifndef WINDOW_H
#define WINDOW_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "Settings.h" 

using namespace std;

class Window
{

	private:
		Window();
		Window(Window const&){};
		Window& operator=(Window const&){};
		static Window* pInstance;
		Settings* settings;
		SDL_Surface* screen;
		SDL_Surface* tileset;
		vector<SDL_Rect> sprites;
		bool init_screen();
		SDL_Surface* load_image( string filename );
		void clip_tiles();

	public:
		static Window* CreateInstance();
		SDL_Surface* get_screen();
		SDL_Surface* get_tileset();
		vector<SDL_Rect> get_sprites();
		bool update();
		void clean();

};

#endif
