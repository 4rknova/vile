#include <cstdio>
#include <cmath>
#include <omp.h>
#include "ray.h"
#include "texture.h"
#include "renderer.h"
#include "time.h"

double Renderer::render(Framebuffer &fb, Camera &cam, Map &map)
{
	fb.clear();

	// Texture - Just for testing
	Texture tex1(16,16);
	Texture tex2(16,16);
	texture_proc_x(tex1);
	texture_proc_xor(tex2, 1);

	// timing
	double time_start = get_ticks();

    #pragma omp parallel for schedule(dynamic, 1)
	for (unsigned int x = 0; x < fb.width; x++)
    {
    	double coef = 2 * x / double(fb.width) - 1;

		Ray ray;
		ray.position  = cam.position;
		ray.direction = Vector2(cam.direction.x + cam.plane.x * (coef), cam.direction.y + cam.plane.y * (coef));

		// Length of ray from one x or y-side to next x or y-side
		// Calculation uses pythagorean and similar triangles
        Vector2 sqdr = Vector2(ray.direction.x * ray.direction.x, ray.direction.y * ray.direction.y);
		Vector2 grid_delta = Vector2(sqrt(1 + sqdr.y / sqdr.x), sqrt(1 + sqdr.x / sqdr.y));

		// Step direction [ +1 | -1 ]
		// If the ray.directionection has a negative [x|y]-component, step[X|Y] is -1.
		// if the ray.directioneciton has a positive [x|y]-component, step[X|Y] is +1.
		// If the x-component is 0, it doesn't matter what value step[X|Y] has since it'll then be unused.
		Vector2 step = Vector2(ray.direction.x < 0 ? -1 : 1, ray.direction.y < 0 ? -1 : 1);

        // Grid position
        Vector2 grid_position = Vector2((int)ray.position.x, (int)ray.position.y);

        // Length of ray from current position to next x or y-side
        Vector2 grid_distance = Vector2(
            (step.x == -1 ? (ray.position.x - grid_position.x) : (grid_position.x + 1.0 - ray.position.x)) * grid_delta.x,
		    (step.y == -1 ? (ray.position.y - grid_position.y) : (grid_position.y + 1.0 - ray.position.y)) * grid_delta.y
        );

		int hit = 0; // was there a wall hit?
		int side;  	 // was a North / Southi (0) or a East / West wall (1) hit?

        int id = 0;

		// DDA - Digital Differential Analysis
		while (!hit)
		{
			// jump to next square in x direction or y direction
			int move_on_x = grid_distance.x < grid_distance.y;

            if (move_on_x) {
                grid_distance.x += grid_delta.x;
    		    grid_position.u += step.x;
                side = 0;
            }
            else {
                grid_distance.y += grid_delta.y;
			    grid_position.v += step.y;
                side = 1;
            }

            id = *map.block((int)grid_position.x, (int)grid_position.y);

            if (id != 0) hit = 1;
		}

		// Calculate distance to wall (correctly projected to cancel fisheye effect)
        // We compensate for the 1 that was added above while stepping
		double distance = (side == 0 ?
			fabs((grid_position.x - ray.position.x + (1 - step.x) / 2) / ray.direction.x):
			fabs((grid_position.y - ray.position.y + (1 - step.y) / 2) / ray.direction.y));

		// calculate height of line to draw on screen
		int column_size = (int)fabs(fb.height / distance);

		// calculate lowest and highest pixel to fill in the current column
		int column_start = fb.height / 2 - column_size / 2;
		int column_end   = column_start + column_size;
		if (column_start < 0) column_start = 0;
		if (column_end >= (int)fb.height) column_end = fb.height - 1;

        // Depth Cue
        float cue = 1. / distance;

		// Texture Mapping
        int texX = 0;
		// Calculate the exact hit point
		double wallX = (side == 1 ?
			ray.position.x + ((grid_position.v - ray.position.y + (1 - step.v) / 2) / ray.direction.y) * ray.direction.x:
			ray.position.y + ((grid_position.u - ray.position.x + (1 - step.u) / 2) / ray.direction.x) * ray.direction.y);
		wallX -= floor(wallX);

		// x coordinate on the texture
		texX = int(wallX * double(tex1.width));
   		if((side == 0 && ray.direction.x > 0) || (side == 1 && ray.direction.y < 0)) texX = tex1.width - texX - 1;

		// draw
		if (cue > 1.0) cue = 1.0;
		for (int y = 0; y < (int)fb.height; y++)
		{
			pixel32_t *p = fb.pixel(x, y);

			if (y < column_start)			// Ceiling
				*p = 0x777777FF;
			else if (y < column_end) {		// Wall
                // Texturing
                {
					int d = y * 256 - fb.height * 128 + column_size * 128; // 256 and 128 factors to avoid floats
                    switch (id)
                    {
                        case 1:  *p = *tex1.pixel(texX, ((d * tex1.height) / column_size) / 256); break;
	    				case 2:  *p = *tex2.pixel(texX, ((d * tex2.height) / column_size) / 256); break;
	    				default: *p = *tex2.pixel(texX, ((d * tex2.height) / column_size) / 256); break;
                    }

					// Depth cue test
					float red = get_pixel32_r(*p);
					float grn = get_pixel32_g(*p);
					float blu = get_pixel32_b(*p);

					red = red * cue;
    				grn = grn * cue;
					blu = blu * cue;

					*p = rgba_c_to_pixel32((char)red, (char)grn, (char)blu, 255);
				}
			}
			else						// Floor
            {
				*p = 0x222222FF;
			}
		}
/*
        #pragma omp critical
        {
            printf("\r[%i/%i] FPS: %i  ", omp_get_thread_num(), omp_get_num_threads(), (int)(1000. / (get_ticks() - time_start))); fflush(stdout);
        }
*/
    }

	return (get_ticks() - time_start) / 1000.;
}
