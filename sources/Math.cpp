#include "./../includes/Math.h"

mat4 matMult(mat4 a, mat4 b) {
	mat4	matrix;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			matrix.data[y][x] = a.data[y][0] * b.data[0][x] + a.data[y][1] * b.data[1][x] + a.data[y][2] * b.data[2][x] + a.data[y][3] * b.data[3][x];
		}
	}

	return (matrix);
}

mat4 identityMat(float id) {
	mat4	matrix;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			matrix.data[y][x] = 0;
		}
	}

	matrix.data[0][0] = id;
	matrix.data[1][1] = id;
	matrix.data[2][2] = id;
	matrix.data[3][3] = 1;

	return (matrix);
}

mat4 scaletyMat(float x, float y, float z) {
	mat4	matrix;

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			matrix.data[y][x] = 0;
		}
	}

	matrix.data[0][0] = x;
	matrix.data[1][1] = y;
	matrix.data[2][2] = z;
	matrix.data[3][3] = 1;

	return (matrix);
}