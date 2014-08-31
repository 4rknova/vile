#ifndef VILE_TEXTURE_H_INCLUDED
#define VILE_TEXTURE_H_INCLUDED

#include "pixel.h"

class Texture
{
	public:
		Texture(unsigned int width, unsigned int height);
		~Texture();

		void clear();

		// Pixels are being accessed starting with (0,0).
		// In case of out of bounds exceptions, the last pixel is affected.
		pixel32_t *pixel(unsigned int x, unsigned int y);
        pixel32_t *pixel(float x, float y);

		const unsigned int width;
		const unsigned int height;

		pixel32_t *pixels;
};

void texture_proc_xor(Texture &tex, bool inverse = 0);
void texture_proc_brick(Texture &tex);
void texture_proc_x(Texture &tex);

#endif /* VILE_TEXTURE_H_INCLUDED */
