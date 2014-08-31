#ifndef VILE_SCREEN_H_INCLUDED
#define VILE_SCREEN_H_INCLUDED

#include <SDL/SDL.h>
#include "fb.h"

class Screen
{
	public:
		Screen(Framebuffer &fb);
		~Screen();

		unsigned int init();
		unsigned int deinit();
		unsigned int update();
		unsigned int update(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
		unsigned int flip();
		void fullscreen(unsigned int state);

	private:
		Framebuffer* m_fb;
		SDL_Surface* m_screen;
};

#endif /* VILE_SCREEN_H_INCLUDED */
