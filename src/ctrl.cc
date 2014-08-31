#include <cmath>
#include <SDL/SDL.h>
#include "ctrl.h"

void Controller::update_camera(Framebuffer &fb, Camera &cam, Map &map, float time)
{
	SDL_PumpEvents();
	Uint8 *keystate = SDL_GetKeyState(NULL);

    double moveSpeed = time * 4.0; //the constant value is in squares/second

	if (keystate[SDLK_LSHIFT]) {
        moveSpeed *= 4.0;
    }

	if (keystate[SDLK_w])
	{
		if(*map.block(int(cam.position.x + cam.direction.x * moveSpeed), int(cam.position.y)) == false)
            cam.position.x += cam.direction.x * (moveSpeed);
		if(*map.block(int(cam.position.x), int(cam.position.y + cam.direction.y * moveSpeed)) == false)
            cam.position.y += cam.direction.y * (moveSpeed);
	}
	if (keystate[SDLK_s])
	{
		if(*map.block(int(cam.position.x - cam.direction.x * moveSpeed), int(cam.position.y)) == false)
            cam.position.x -= cam.direction.x * moveSpeed;
		if(*map.block(int(cam.position.x), int(cam.position.y - cam.direction.y * moveSpeed)) == false)
            cam.position.y -= cam.direction.y * moveSpeed;
	}
	if (keystate[SDLK_a])
	{
	}
	if (keystate[SDLK_d])
	{
	}

	// Mouse
	Vector2 scr_center, mouse;
	scr_center.x = fb.width / 2;
	scr_center.y = fb.height / 2;

	int mx,my;
	SDL_GetMouseState(&mx,&my);

	mouse.x = mx;
	mouse.y = my;

	float mousedx = 0.3 *  -((mouse.x - scr_center.x) / scr_center.x) / (M_PI*4);
	{
		double oldDirX = cam.direction.x;
		cam.direction.x = oldDirX * cos(mousedx) - cam.direction.y * sin(mousedx);
		cam.direction.y = oldDirX * sin(mousedx) + cam.direction.y * cos(mousedx);
		double oldPlnX = cam.plane.x;
		cam.plane.x = oldPlnX * cos(mousedx) - cam.plane.y * sin(mousedx);
		cam.plane.y = oldPlnX * sin(mousedx) + cam.plane.y * cos(mousedx);
	}
}


void Controller::update_screen(Screen &screen)
{
	SDL_PumpEvents();
	Uint8 *keystate = SDL_GetKeyState(NULL);

	if (keystate[SDLK_f]) screen.fullscreen(3);

/*
    if (keystate[SDLK_SPACE]) {
        printf("\nMAP - pos: %ix%i\n", (int)cam.position.x, (int)cam.position.y);

        for (int x = 0; x < map.get_width(); ++x) {
            printf("\n");

             for (int y = 0; y < map.get_height(); ++y) {
                 int n = y * map.get_width() + x;
                 if ((int)cam.position.x == x && (int)cam.position.y == y) printf("*");
                 else if ((int)(cam.position.x + cam.direction.x) == x && (int)(cam.position.y + cam.direction.y) == y) printf(".");
                 else if (*map.block(x,y) == 0) printf(" ");
                 else printf("%i", *map.block(x, y));
             }
        }
        printf("\n");
        fflush(stdout);
    }
*/
}

bool Controller::exit_flag()
{
	SDL_PumpEvents();
	Uint8 *keystate = SDL_GetKeyState(NULL);

    return keystate[SDLK_ESCAPE];
}
