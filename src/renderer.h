#ifndef VILE_RENDERER_H_INCLUDED
#define VILE_RENDERER_H_INCLUDED

#include "fb.h"
#include "camera.h"
#include "map.h"

class Renderer
{
	public:
        double render(Framebuffer &fb, Camera &cam, Map &map);
};

#endif /* VILE_RENDERER_H_INCLUDED */
