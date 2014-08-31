#include <string>
#include "screen.h"
#include "pixel.h"

Screen::Screen(Framebuffer &fb):
	m_fb(&fb),
	m_screen(NULL)
{}

Screen::~Screen()
{}

unsigned int Screen::init()
{
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
		return 1;

	// setup the window
	m_screen = SDL_SetVideoMode( m_fb->width, m_fb->height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	if(!m_screen)
		return 1;

	// Continuous input
	SDL_EnableKeyRepeat(10,SDL_DEFAULT_REPEAT_INTERVAL);
	// setup the window caption
	std::string caption = "Vile v";
	caption.append(VILE_VERSION);
	SDL_WM_SetCaption(caption.c_str(), NULL);
	return 0;
}


unsigned int Screen::deinit()
{
	SDL_Quit(); // This will release m_window as well
	return 0;
}

unsigned int Screen::update()
{
	return update(0, 0, m_fb->width, m_fb->height);
}

unsigned int Screen::update(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
	// check if the surface must be locked
	if(SDL_MUSTLOCK(m_screen))
	{
		SDL_LockSurface(m_screen);
	}

	// check for "out of bounds" and inverse mapping errors
	if ((x0 > m_fb->width) 
		|| (x1 > m_fb->width) 
		|| (y0 > m_fb->height) 
		|| (y1 > m_fb->height)
		|| (x0 > x1)
		|| (y0 > y1))
		return 1;

	// convert the pixels to 32 bit
	pixel32_t *pixels = (pixel32_t *)m_screen->pixels;

	// set the pixels
	for(unsigned int y = y0; y < y1; y++)
	{
		for(unsigned int x = x0; x < x1; x++)
		{
			pixel32_t p = *(m_fb->pixel(x, y));
			
			pixels[(y * m_screen->w) + x] = 
				SDL_MapRGB( 
						m_screen->format, 
						get_pixel32_r(p), 
						get_pixel32_g(p), 
						get_pixel32_b(p));
		}
	}

	// check if the surface must be unlocked
	if(SDL_MUSTLOCK(m_screen))
	{
		SDL_UnlockSurface(m_screen);
	}

	// flip the buffer
	flip();

	return 0;
}

unsigned int Screen::flip()
{
	// update the screen
	if (SDL_Flip(m_screen) == -1)
	{
		return 1;
	}
	return 0;
}

void Screen::fullscreen(unsigned int state) 
{
	if ((!(m_screen->flags & SDL_FULLSCREEN) && state == 1) || 
		((m_screen->flags & SDL_FULLSCREEN) && state == 0) ||
		(state > 1))
		SDL_WM_ToggleFullScreen(m_screen);

	SDL_ShowCursor(state ? SDL_DISABLE : SDL_ENABLE);
} 
