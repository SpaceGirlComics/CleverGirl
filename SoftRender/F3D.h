#ifndef F3D_H
#define F3D_H
#include "Matrices.h"
typedef struct _FCAMERA
{
	float x, y, z;
	float roll, pitch, yaw;
	float fov;
}FCAMERA, * LPFCAMERA;

typedef struct _FTARGET
{
	unsigned char* buffer;
	int width, height, depth;
}FTARGET, * LPFTARGET;

typedef struct _POLYGON
{
	VERTEX v[3];
}POLYGON, * LPPOLYGON;
void matrixByVerex(LPMATRIX _m, LPVERTEX _v);
LPFTARGET initializeF3D(int _xres, int _yres, int _depth, unsigned char* _buffer, float _fov);
void draw(char* _buffer, int _length, LPFTARGET _back);
int destroyF3D();

#endif
