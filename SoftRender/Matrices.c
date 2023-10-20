#include <stdlib.h>
#include <math.h>
#include "Matrices.h"

// Functions whose name ends with "Matrix" (excluding destroyMatrix) must follow these rules:
// 1. Any Matrix (or pointer to) passed as a parameter is unaltered by the function
// 2. The function returns a matrix that must be disposed of by the destroyMatrix function
// m[Row][Column]

MATRIX createMatrix(int _rows, int _columns)
{
	MATRIX m;
	if (_columns < 1 || _rows < 1)
	{
		m.cols == m.rows == 0;
		m.mat = (float**)0;
		return(m);
	}
	m.cols = _columns;
	m.rows = _rows;
	m.mat = (float**)calloc(_rows, sizeof(float*));

	for (int x = 0; x < m.rows; x++)
	{
		m.mat[x] = (float*)calloc(_columns, sizeof(float));
	}
	return(m);
}

MATRIX copyMatrix(LPMATRIX _m)
{
	if (_m)
	{
		MATRIX i = createMatrix(_m->rows, _m->cols);
		for (int x = 0; x < i.rows; x++)
		{
			for (int y = 0; y < i.cols; y++)
			{
				i.mat[x][y] = _m->mat[x][y];
			}
		}
		return(i);
	}
	else
	{
		return(createMatrix(0, 0));
	}
}

MATRIX minorMatrix(LPMATRIX _m, int _x, int _y)
{
	if ((!_m) || _m->rows < 1 || _x > _m->rows || _x < 0 || _m->cols < 1 || _y > _m->cols || _y < 0)
	{
		return(createMatrix(0, 0));
	}
	MATRIX a = createMatrix(_m->rows - 1, _m->rows - 1);
	int b = 0, c = 0;
	for (int x = 0; x < _m->rows; x++)
	{
		if (x != _y)
		{
			for (int y = 0; y < _m->cols; y++)
			{
				if (y != _x)
				{
					a.mat[b][c++] = _m->mat[x][y];
				}
			}
			c = 0;
			b++;
		}
	}
	return(a);
}

MATRIX cofactorMatrix(LPMATRIX _m)
{
	MATRIX m = createMatrix(_m->rows, _m->cols);
	MATRIX temp;
	int factor = 1;
	for (int x = 0; x < m.rows; x++)
	{
		for (int y = 0; y < m.cols; y++)
		{
			temp = minorMatrix(_m, y, x);
			m.mat[x][y] = det(&temp);
			if ((x + y) % 2)
			{
				m.mat[x][y] *= -1;
			}
			destroyMatrix(temp);
		}
	}
	return(m);
}

MATRIX transposeMatrix(LPMATRIX _m)
{
	MATRIX i = createMatrix(_m->cols, _m->rows);
	for (int x = 0; x < i.rows; x++)
	{
		for (int y = 0; y < i.cols; y++)
		{
			i.mat[x][y] = _m->mat[y][x];
		}
	}
	return(i);
}

MATRIX adjointMatrix(LPMATRIX _m)
{
	MATRIX x = cofactorMatrix(_m);
	MATRIX y = transposeMatrix(&x);
	destroyMatrix(x);
	return(y);
}

MATRIX inverseMatrix(LPMATRIX _m)
{
	if ((!_m) || _m->cols != _m->rows)
	{
		return(createMatrix(0, 0));
	}
	MATRIX left = copyMatrix(_m);
	MATRIX right = createMatrix(_m->rows, _m->cols);
	loadIdentity(&right);

	float pivot;
	float factor;

	for (int x = 0; x < left.rows; x++)
	{
		pivot = left.mat[x][x];
		for (int y = 0; y < left.cols; y++)
		{
			left.mat[x][y] /= pivot;
			right.mat[x][y] /= pivot;
		}
		for (int y = 0; y < left.rows; y++)
		{
			if (y != x)
			{
				factor = left.mat[y][x];
				for (int z = 0; z < left.cols; z++)
				{
					left.mat[y][z] -= factor * left.mat[x][z];
					right.mat[y][z] -= factor * right.mat[x][z];
				}
			}
		}
	}
	destroyMatrix(left);
	return(right);
}

MATRIX addMatrix(LPMATRIX _left, LPMATRIX _right)
{
	MATRIX i;
	if (_left->cols == _right->cols && _left->rows == _right->rows)
	{
		i = createMatrix(_left->rows, _left->cols);
		for (int x = 0; x < i.rows; x++)
		{
			for (int y = 0; y < i.cols; y++)
			{
				i.mat[x][y] = _left->mat[x][y] + _right->mat[x][y];
			}
		}
	}
	else
	{
		i = createMatrix(0, 0);
	}
	return(i);
}

MATRIX subtractMatrix(LPMATRIX _left, LPMATRIX _right)
{
	MATRIX i;
	if (_left && _right && _left->cols == _right->cols && _left->rows == _right->rows)
	{
		i = createMatrix(_left->rows, _left->cols);
		for (int x = 0; x < i.rows; x++)
		{
			for (int y = 0; y < i.cols; y++)
			{
				i.mat[x][y] = _left->mat[x][y] - _right->mat[x][y];
			}
		}
	}
	else
	{
		i = createMatrix(0, 0);
	}
	return(i);
}

MATRIX scaleMatrix(LPMATRIX _m, float _scalar)
{
	if (!_m)
	{
		return(createMatrix(0, 0));
	}
	MATRIX i = createMatrix(_m->rows, _m->cols);
	for (int x = 0; x < i.rows; x++)
	{
		for (int y = 0; y < i.cols; y++)
		{
			i.mat[x][y] = _m->mat[x][y] * _scalar;
		}
	}
	return(i);
}

MATRIX multiplyMatrix(LPMATRIX _left, LPMATRIX _right)
{
	if ((!_left) || (!_right) || _left->cols < 1 || _left->rows < 1 || _right->cols < 1 || _right->rows < 1 || _left->cols != _right->rows)
	{
		return(createMatrix(0, 0));
	}
	MATRIX out = createMatrix(_left->rows, _right->cols);

	for (int y = 0; y < _left->rows; y++)
	{
		for (int x = 0; x < _right->cols; x++)
		{
			for (int z = 0; z < _left->cols; z++)
			{
				out.mat[y][x] += _left->mat[y][z] * _right->mat[z][x];
			}
		}
	}
	return(out);
}

float det(LPMATRIX _m)
{
	float d = 0.0f;
	MATRIX m;
	int w = 0, s = 1;
	if ((!_m) || _m->rows != _m->cols || _m->rows < 1)
	{
		return(d);
	}
	if (_m->rows == _m->cols)
	{
		if (_m->rows == 1)
		{
			return(_m->mat[0][0]);
		}
		if (_m->rows == 2)
		{
			return(_m->mat[0][0] * _m->mat[1][1] - _m->mat[0][1] * _m->mat[1][0]);
		}
		for (int x = 0; x < _m->rows; x++)
		{
			m = minorMatrix(_m, x, 0);
			d += s * _m->mat[0][x] * det(&m);
			s *= -1;
			destroyMatrix(m);
		}
	}
	return(d);
}

void destroyMatrix(MATRIX _m)
{
	if (_m.mat)
	{
		for (int x = 0; x < _m.rows; x++)
		{
			if (_m.mat[x])
			{
				free(_m.mat[x]);
				_m.mat[x] = (float*)0;
			}
		}
		free(_m.mat);
	}
	_m.rows = 0;
	_m.cols = 0;
	_m.mat = NULL;
}

LPMATRIX createLPMatrix(int _rows, int _columns)
{
	LPMATRIX m = malloc(sizeof(MATRIX));

	m->rows = _rows;
	m->cols = _columns;

	m->mat = calloc(_rows, sizeof(float*));
	for (int x = 0; x < m->rows; x++)
	{
		m->mat[x] = calloc(_columns, sizeof(float));
	}
	return(m);
}

void translate(LPMATRIX _m, float _x, float _y, float _z)
{
	if (_m)
	{
		MATRIX m = createMatrix(4, 4);
		MATRIX out;
		loadIdentity(&m);
		m.mat[0][3] = _x;
		m.mat[1][3] = _y;
		m.mat[2][3] = _z;
		out = multiplyMatrix(_m, &m);
		destroyMatrix(*_m);
		_m->cols = out.cols;
		_m->mat = out.mat;
		_m->rows = out.rows;
		destroyMatrix(m);
	}
}

void rotate(LPMATRIX _in, float _roll, float _pitch, float _yaw)
{
	if (_in)
	{
		MATRIX temp = createMatrix(4, 4);
		float sr = sinf(_roll);
		float cr = cosf(_roll);
		float sp = sinf(_pitch);
		float cp = cosf(_pitch);
		float sy = sinf(_yaw);
		float cy = cosf(_yaw);
		temp.mat[0][0] = cy * cp;
		temp.mat[0][1] = cy * sp * sr - sy * cr;
		temp.mat[0][2] = cy * sp * cr + sy * sr;
		temp.mat[1][0] = sy * cp;
		temp.mat[1][1] = sy * sp * sr + cy * cr;
		temp.mat[1][2] = sy * sp * cr - cy * sp;
		temp.mat[2][0] = -sp;
		temp.mat[2][1] = cp * sr;
		temp.mat[2][2] = cp * cr;
		temp.mat[3][3] = 1.0f;

		MATRIX out = multiplyMatrix(&temp, _in);
		destroyMatrix(*_in);
		_in->cols = out.cols;
		_in->mat = out.mat;
		_in->rows = out.rows;
		destroyMatrix(temp);
	}
}

void scale(LPMATRIX _in, float _scalar)
{
	if (_in)
	{
		for (int y = 0; y < _in->rows; y++)
		{
			for (int x = 0; x < _in->cols; y++)
			{
				_in->mat[y][x] *= _scalar;
			}
		}
	}
}

void loadIdentity(LPMATRIX _m)
{
	for (int x = 0; x < _m->rows; x++)
	{
		for (int y = 0; y < _m->cols; y++)
		{
			if (y == x)
			{
				_m->mat[x][y] = 1;
			}
			else
			{
				_m->mat[x][y] = 0;
			}
		}
	}
}

void destroyLPMatrix(LPMATRIX _m)
{
	if (_m)
	{
		for (int x = 0; x < _m->rows; x++)
		{
			if (_m->mat[x])
			{
				free(_m->mat[x]);
			}
		}
		if (_m->mat)
		{
			free(_m->mat);
		}
		free(_m);
	}
}
