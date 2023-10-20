#include "Vertices.h"

void scaleVertex(LPVERTEX _v, float _scalar)
{
	_v->x *= _scalar;
	_v->y *= _scalar;
	_v->z *= _scalar;
	_v->w *= _scalar;
}

void addVertices(LPVERTEX _l, LPVERTEX _r)
{
	_l->x += _r->x;
	_l->y += _r->y;
	_l->z += _r->z;
}
