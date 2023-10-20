#include "Vertices.h"

VERTEX matrixVertexMultiply(LPMATRIX _m, LPVERTEX _v)
{
	VERTEX a;
	a.color = _v->color;
	a.s = _v->s;
	a.t = _v->t;
	a.x = _m->mat[0][0] * _v->x + _m->mat[0][1] * _v->y + _m->mat[0][2] * _v->z + _m->mat[0][3] * _v->w;
	a.y = _m->mat[1][0] * _v->x + _m->mat[1][1] * _v->y + _m->mat[1][2] * _v->z + _m->mat[1][3] * _v->w;
	a.z = _m->mat[2][0] * _v->x + _m->mat[2][1] * _v->y + _m->mat[2][2] * _v->z + _m->mat[2][3] * _v->w;
	a.w = _m->mat[3][0] * _v->x + _m->mat[3][1] * _v->y + _m->mat[3][2] * _v->z + _m->mat[3][3] * _v->w;

	return(a);
}

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
