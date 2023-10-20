#include <stdlib.h>
#include <math.h>
//#include "Matricies.h"
#include "F3D.h"

MATRIX view;
MATRIX projection;
int mode;

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

LPFTARGET initializeF3D(int _xres, int _yres, int _depth, unsigned char* _buffer, float _fov)
{
	LPFTARGET b = malloc(sizeof(FTARGET));
	b->depth = _depth;
	b->width = _xres;
	b->height = _yres;
	b->buffer = _buffer;
	mode = 0x00000001;

	view = createMatrix(4, 4);
	loadIdentity(&view);

	float ar = (float)_xres / (float)_yres;
	float fr = (3.14 * _fov) / 180;
	float it = 1 / tanf(fr / 2.0f);
	float far = 100.0f;
	float near = 0.1f;
	float fs = far / (far - near);
	float ns = -(far * near) / (far - near);

	projection = createMatrix(4, 4);
	projection.mat[0][0] = ar * it;
	projection.mat[1][1] = it;
	projection.mat[2][2] = fs;
	projection.mat[2][3] = ns;
	projection.mat[3][2] = 1;
	return(b);
}

void draw(char* _buffer, int _length, LPFTARGET _back)
{
	VERTEX xyz[3];
	xyz[0].x = 10.0f;
	xyz[0].y = 10.0f;
	xyz[0].z = 20.0f;
	xyz[0].w = 1.0f;

	xyz[1].x = 20.0f;
	xyz[1].y = 10.0f;
	xyz[1].z = 20.0f;
	xyz[1].w = 1.0f;

	xyz[2].x = 15.0f;
	xyz[2].y = 20.0f;
	xyz[2].z = 20.0f;
	xyz[2].w = 1.0f;

	int x[3];
	int y[3];

	for (int a = 0; a < 3; a++)
	{
		VERTEX cam = matrixVertexMultiply(&view, &xyz[a]);
		VERTEX clip = matrixVertexMultiply(&projection, &cam);
		scaleVertex(&clip, 1 / clip.w);
		x[a] = clip.x;
		y[a] = clip.y;
	}
	*(unsigned int*)(&_back->buffer[y[0] * _back->width * _back->depth + x[0] * _back->depth]) = 0xff0000ff;
	*(unsigned int*)(&_back->buffer[y[1] * _back->width * _back->depth + x[1] * _back->depth]) = 0xffff0000;
	*(unsigned int*)(&_back->buffer[y[2] * _back->width * _back->depth + x[2] * _back->depth]) = 0xff00ff00;
}

int destroyF3D()
{
	if (view.mat)
	{
		destroyMatrix(view);
	}
	if (projection.mat)
	{
		destroyMatrix(projection);
	}
	return(0);
}
