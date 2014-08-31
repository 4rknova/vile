#include "texture.h"

Texture::Texture(unsigned int width, unsigned int height): 
	width(width > 0 ? width : 1), 
	height(height > 0 ? height : 1),
	pixels(new pixel32_t[width * height])
{}

Texture::~Texture()
{
	delete [] pixels;
}

pixel32_t *Texture::pixel(unsigned int x, unsigned int y)
{
	unsigned int n = y * width + x;
	return (x < width) ? ((y < height) ? &pixels[n] : &pixels[width * height - 1])  : &pixels[width * height - 1];
}

pixel32_t *Texture::pixel(float x, float y)
{
    unsigned int nx = x * width;
    unsigned int ny = y * width;

	return pixel(nx, ny);
}


void Texture::clear()
{
	for (unsigned int y = 0; y < height; y++)
		for (unsigned int x = 0; x < width; x++)
		{
			pixel32_t *p = pixel(x, y);
			*p = 0;
		}
}

void texture_proc_xor(Texture &tex, bool inverse)
{
	for (unsigned int y = 0; y < tex.height; y++)
		for (unsigned int x = 0; x < tex.width; x++)
		{
			int xorc = x ^ y;
			pixel32_t *p = tex.pixel(x, y);
			*p = rgba_c_to_pixel32(xorc, xorc, xorc, 255);
			if (inverse)
				*p ^= 0x555555FF;
		}
}

void texture_proc_x(Texture &tex)
{
	for (unsigned int y = 0; y < tex.height; y++)
		for (unsigned int x = 0; x < tex.width; x++)
		{
			pixel32_t *p = tex.pixel(x, y);
			*p = 65536 * 254 * (x != y && x != tex.width - y);
		}
}

void texture_proc_brick(Texture &tex)
{
	for (unsigned int y = 0; y < tex.height; y++)
		for (unsigned int x = 0; x < tex.width; x++)
		{
			pixel32_t *p = tex.pixel(x, y);
			*p = 65536 * 192 * (x % 16 && y % 16);
		}
}
