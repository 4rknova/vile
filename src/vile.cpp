#include <cstdio>
#include <cmath>

#include "fb.h"
#include "screen.h"
#include "vector.h"
#include "camera.h"
#include "ray.h"
#include "time.h"
#include "renderer.h"
#include "ctrl.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s mapfile\n", argv[0]);
		return 1;
	}

    Controller controller;
	Camera cam;
    Renderer renderer;
	Framebuffer fb(800, 600);
	Screen screen(fb);

	if (screen.init()) {
		printf("Failed to create the SDL window.\n");
		return 1;
	}

    Map map(argv[1]);
	map.load(cam);

	while (1) {
        double time = renderer.render(fb, cam, map);
		screen.update();
        controller.update_screen(screen);
        controller.update_camera(fb, cam, map, time);

        printf("\rFPS: %.3f", 1 / time);
        fflush(stdout);

        if (controller.exit_flag()) break;
	}

	screen.fullscreen(0);
	return 0;
}
