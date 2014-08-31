#ifndef VILE_MAP_H_INCLUDED
#define VILE_MAP_H_INCLUDED

#include <string>
#include "camera.h"

/*
	The map format is as follows:
	1. %ix%i
	2.
	3. %f %f
	4. %f %f
	5. %f %f
	6.
	3. %i,%i,%i,..
	.. %i,%i,%i,..

	line 1: width and height
	line 2: empty
	line 3: camera position
	line 4: camera direction
	line 5: camera view plane

	The rest of the lines contain the map data
*/

class Map
{
	public:
		Map(const char* file);
		~Map();

		// Parse the map file
		int load(Camera &cam);

		// Access a block by its coordinates
		int *block(unsigned int x, unsigned int y);

        int get_width();
        int get_height();
        int get_depth();

	protected:
		const std::string m_source;
		unsigned int m_width;
		unsigned int m_height;
        unsigned int m_depth;

        int load_map(const char *source);
        int load_cam(const char *source, Camera &cam);

		int *m_blocks;
};

#endif /* VILE_MAP_H_INCLUDED */
