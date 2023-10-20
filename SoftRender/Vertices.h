#ifndef VERTICES_H
#define VERTICES_H

typedef struct _VERTEX
{
	float x, y, z, w;
	unsigned int color;
	float s, t;
}VERTEX, * LPVERTEX;

void scaleVertex(LPVERTEX _v, float _scalar);
void addVertices(LPVERTEX _l, LPVERTEX _r);

#endif
