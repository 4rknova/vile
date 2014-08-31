#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "ncf.h"
#include "map.h"

#define LVL_MAP "map"
#define LVL_CAM "cam"

#define LVL_CAM_ROOT "camera"
#define LVL_CAM_POS  "position"
#define LVL_CAM_DIR  "direction"
#define LVL_CAM_PLN  "plane"
#define LVL_VAL_X    "x"
#define LVL_VAL_Y    "y"

Map::Map(const char* file)
	: m_source(file)
    , m_width(0)
    , m_height(0)
    , m_depth(0)
    , m_blocks(NULL)
{}

Map::~Map()
{
	if (m_blocks) free(m_blocks);
}

int Map::get_width()
{
    return m_width;
}

int Map::get_height()
{
    return m_height;
}

int Map::get_depth()
{
    return m_depth;
}

int Map::load(Camera &cam)
{
    std::string src_map = m_source + "/" + LVL_MAP;
    std::string src_cam = m_source + "/" + LVL_CAM;

    load_cam(src_cam.c_str(), cam);
    load_map(src_map.c_str());

    return 0;
}

int Map::load_cam(const char *source, Camera &cam)
{
	printf("Reading camera data from %s\n", source);

    NCF cfg;
    cfg.set_source(source);
    int res = cfg.parse();

	if (res) {
		fprintf(stderr, "Error: Failed to open : %s\n", source);
		return 1;
    }
    else {
        cam.position.x  = Util::String::to_float(cfg.get_group_by_name(LVL_CAM_ROOT)->get_group_by_name(LVL_CAM_POS)->get_property_by_name(LVL_VAL_X));
        cam.position.y  = Util::String::to_float(cfg.get_group_by_name(LVL_CAM_ROOT)->get_group_by_name(LVL_CAM_POS)->get_property_by_name(LVL_VAL_Y));
        cam.direction.x = Util::String::to_float(cfg.get_group_by_name(LVL_CAM_ROOT)->get_group_by_name(LVL_CAM_DIR)->get_property_by_name(LVL_VAL_X));
        cam.direction.y = Util::String::to_float(cfg.get_group_by_name(LVL_CAM_ROOT)->get_group_by_name(LVL_CAM_DIR)->get_property_by_name(LVL_VAL_Y));
		cam.plane.x     = Util::String::to_float(cfg.get_group_by_name(LVL_CAM_ROOT)->get_group_by_name(LVL_CAM_PLN)->get_property_by_name(LVL_VAL_X));
        cam.plane.y     = Util::String::to_float(cfg.get_group_by_name(LVL_CAM_ROOT)->get_group_by_name(LVL_CAM_PLN)->get_property_by_name(LVL_VAL_Y));
	}

	return 0;
}

int Map::load_map(const char *source)
{
	printf("Reading map data from %s\n", source);

	FILE *file = fopen (source, "r");
	if (file != NULL)
	{
		// Read the map dimensions
		int w = 0, h = 0;
		if(fscanf(file, "%ix%i", &w, &h)!=2 || w<=0 || h<=0)
		{
			// Something went wrong
			fprintf(stderr, "Error: Failed to read map dimensions: %s..\n", m_source.c_str());
			fclose(file);
			return 1;
		}

        printf("Map dimensions: %ix%i\n", w, h);
        m_width  = w;
        m_height = h;

		// Allocate memory for map data
		m_blocks = (int*)malloc(w*h*sizeof(int));

		if(!m_blocks)
		{
			// Something went wrong
			fprintf(stderr, "Error: Failed to allocate memory for map data: %s\n", m_source.c_str());
			fclose(file);
			return 1;
		}

		// Read map data
		for (int i=0; i<h; i++)
		{
			for (int y=0; y<w; y++)
			{
				if (fscanf(file, "%i,", &m_blocks[i*w+y]) != 1)
				{
					// Something went wrong
					fprintf(stderr, "Error: Failed to read the map data: %s\n", m_source.c_str());
					fclose(file);
					return 1;
				}
			}
		}

		fclose ( file );
	}
	else
	{
		fprintf(stderr, "Error: Failed to open map: %s\n", source);
		return 1;
	}

	return 0;
}

int def_val = 0;

int *Map::block(unsigned int x, unsigned int y)
{
	unsigned int n = y * m_width + x;
//    printf("\rindex: %i, value: %ix%i = %i %s", n, x, y, m_blocks[n], m_blocks[n] != 0 ? " hit" : ""); fflush(stdout);
	return (x < m_width && y < m_height) ? &m_blocks[n] : &def_val;
}
