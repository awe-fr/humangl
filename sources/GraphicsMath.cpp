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
	mat4 matrix = identityMat(1);

	float angle = tan(FOV / 2 * DEG2RAD);

	matrix.data[0][0] = znear / (znear * angle);
	matrix.data[1][1] = znear / ((znear * angle) / aspect);
	matrix.data[2][2] = -(zfar + znear) / (zfar - znear);
	matrix.data[3][2] = -1;
	matrix.data[2][3] = -(2.0f * zfar * znear) / (zfar - znear);
	matrix.data[3][3] = 0;

	return (matrix);
}

mat4 rotationMatX(float degree) {
	mat4 matrix = identityMat(1);

	float angle = degree * DEG2RAD;

	matrix.data[1][1] = cos(angle);
	matrix.data[1][2] = -sin(angle);
	matrix.data[2][1] = sin(angle);
	matrix.data[2][2] = cos(angle);

	return matrix;
}

mat4 rotationMatY(float degree) {
	mat4 matrix = identityMat(1);

	float angle = degree * DEG2RAD;

	matrix.data[0][0] = cos(angle);
	matrix.data[0][2] = sin(angle);
	matrix.data[2][0] = -sin(angle);
	matrix.data[2][2] = cos(angle);

	return matrix;
}

mat4 rotationMatZ(float degree) {
	mat4 matrix = identityMat(1);

	float angle = degree * DEG2RAD;

	matrix.data[0][0] = cos(angle);
	matrix.data[0][1] = -sin(angle);
	matrix.data[1][0] = sin(angle);
	matrix.data[1][1] = cos(angle);

	return matrix;
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