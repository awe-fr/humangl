#include "./../includes/GraphicsMath.h"

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

mat4 projectionMat(float FOV, float aspect, float znear, float zfar) {
	mat4 matrix = identityMat(0);

	matrix.data[0][0] = aspect * (1 / tan(FOV / 2));
	matrix.data[1][1] = (1 / tan(FOV / 2));
	matrix.data[2][2] =	zfar / (zfar - znear);
	matrix.data[2][3] = (-zfar * znear) / (zfar - znear);
	matrix.data[3][2] = 1.0;
	matrix.data[3][3] = 0;

	return (matrix);
}

mat4 viewMat(vec3 eye, vec3 target, vec3 up) {
	vec3 zaxis = vecNormalize(vecSubstract(eye, target));
	vec3 xaxis = vecNormalize(vecCross(up, zaxis));
	vec3 yaxis = vecCross(zaxis, xaxis);

	mat4 orientation = identityMat(1.0f);

	orientation.data[0][0] = xaxis.x; 
	orientation.data[1][0] = xaxis.y;
	orientation.data[2][0] = xaxis.z;
	orientation.data[0][1] = yaxis.x;
	orientation.data[1][1] = yaxis.y; 
	orientation.data[2][1] = yaxis.z;
	orientation.data[0][2] = zaxis.x;
	orientation.data[1][2] = zaxis.y;
	orientation.data[2][2] = zaxis.z;

	mat4 translation = identityMat(1.0f);

	translation.data[3][0] = -eye.x;
	translation.data[3][1] = -eye.y;
	translation.data[3][2] = -eye.z;

	return (matMult(orientation, translation));
}

void populateMat(float buf[16], mat4 m)
{
	for (int y = 0; y < 4; ++y)
		for (int x = 0; x < 4; ++x)
			buf[4 * y + x] = m.data[x][y];
}

vec3 vecCross(vec3 f, vec3 s) {
	vec3 vector;

	vector.x = (f.y * s.z) - (f.z * s.y);
	vector.y = (f.z * s.x) - (f.x * s.z);
	vector.z = (f.x * s.y) - (f.y * s.x);

	return (vector);
}

vec3 vecNormalize(vec3 f) {
	vec3 vector;

	float a = sqrt((f.x * f.x) + (f.y * f.y) + (f.z * f.z));

	vector.x = f.x / a;
	vector.y = f.y / a;
	vector.z = f.z / a;

	return (vector);
}

vec3 vecMultiplication(vec3 f, float s) {
	f.x = f.x * s;
	f.y = f.y * s;
	f.z = f.z * s;
	return (f);
}

vec3 vecAddition(vec3 f, vec3 s) {
	f.x += s.x;
	f.y += s.y;
	f.z += s.z;
	return (f);
}

vec3 vecSubstract(vec3 f, vec3 s) {
	f.x -= s.x;
	f.y -= s.y;
	f.z -= s.z;
	return (f);
}