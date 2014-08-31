#ifndef VILE_CTRL_H_INCLUDED
#define VILE_CTRL_H_INCLUDED

#include "camera.h"
#include "fb.h"
#include "screen.h"
#include "map.h"

class Controller
{
    public:
        void update_camera(Framebuffer &fb, Camera &cam, Map &map, float time);
        void update_screen(Screen &screne);
        bool exit_flag();
};

#endif /* VILE_CTRL_H_INCLUDED */
