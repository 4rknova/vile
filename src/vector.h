#ifndef VILE_VECTOR_H_INCLUDED
#define VILE_VECTOR_H_INCLUDED

class Vector2
{
	public:
		Vector2();
		Vector2(double n0, double n1);

        union {	double x, u; };
        union { double y, v; };
};

#endif /* VILE_VECTOR_H_INCLUDED */
