#ifndef VILE_CAMERA_H_INCLUDED
#define VILE_CAMERA_H_INCLUDED

#include "vector.h"

class Camera
{
	public:
		Camera();

		Vector2 position;
		Vector2 direction;
		Vector2 plane;
};

#endif /* VILE_CAMERA_H_INCLUDED */
