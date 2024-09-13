#include "./../includes/GraphicsMath.h"

mat4 directionMat(vec3 direction) {
	mat4 matrix = identityMat(1);

	vec3 normDir = vecNormalize(direction);
	vec3 right = vecNormalize(vecCross(vec3{0,1,0}, normDir));
	vec3 up = vecNormalize(vecCross(normDir, right));


	matrix.data[0][0] =	right.x;
	matrix.data[0][1] = right.y;
	matrix.data[0][2] = right.z;
	matrix.data[1][0] = up.x;
	matrix.data[1][1] = up.y;
	matrix.data[1][2] = up.z;
	matrix.data[2][0] = normDir.x;
	matrix.data[2][1] = normDir.y;
	matrix.data[2][2] = normDir.z;

	return(matrix);
}

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

mat4 viewMat(vec3 eye, vec3 target, vec3 up) {
	mat4 matrix = identityMat(1);

	vec3 forward = vecNormalize(vecSubstract(eye, target));
	vec3 left = vecNormalize(vecCross(up, forward));
	up = vecCross(forward, left);

	matrix.data[0][0] = left.x; 
	matrix.data[0][1] = left.y;
	matrix.data[0][2] = left.z; 
	matrix.data[1][0] = up.x;
	matrix.data[1][1] = up.y;
	matrix.data[1][2] = up.z;
	matrix.data[2][0] = forward.x;
	matrix.data[2][1] = forward.y;
	matrix.data[2][2] = forward.z;
	matrix.data[0][3] = -vecDot(left, eye);
	matrix.data[1][3] = -vecDot(up, eye);
	matrix.data[2][3] = -vecDot(forward, eye);

	return (matrix);
}

mat4 translationMat(float x, float y, float z) {
	mat4 matrix = identityMat(1);

	matrix.data[0][3] = x;
	matrix.data[1][3] = y;
	matrix.data[2][3] = z;

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

mat4 upcastmat3(mat3 toUp) {
	mat4 matrix = identityMat(1);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			matrix.data[y][x] = toUp.data[y][x];
		}
	}
	
	return (matrix);
}
#include <iostream>
mat3 quatMat(vec3 dir, vec3 orr) {
	vec3 Ndir = vecNormalize(dir);
	vec3 Nup = vecNormalize(orr);
	vec3 NrotAxis = vecCross(Nup, Ndir);
	if (NrotAxis.x != 0 || NrotAxis.y != 0 || NrotAxis.z != 0)
		NrotAxis = vecNormalize(NrotAxis);
	float angle = acos(vecDot(Nup, Ndir));
	float Sangle = sin(angle);
	float Cangle = cos(angle);

	mat3 K = identityMat3(0);

	K.data[0][1] = -NrotAxis.z;
	K.data[0][2] = NrotAxis.y;
	K.data[1][0] = NrotAxis.z;
	K.data[1][2] = -NrotAxis.x;
	K.data[2][0] = -NrotAxis.y;
	K.data[2][1] = NrotAxis.x;

	// std::cout << vecCross(Nup, Ndir).x << " " << vecCross(Nup, Ndir).y << " " << vecCross(Nup, Ndir).z << std::endl;

	// for (int y = 0; y < 3; y++) {
	// 	for (int x = 0; x < 3; x++) {
	// 		std::cout << K.data[y][x] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;

	mat3 K2 = matMult3(K, K);

	mat3 sinK = matMult3Float(K, Sangle);
	mat3 cosK2 = matMult3Float(K2, (1 - Cangle));

	mat3 R = matadd3(identityMat3(1), matadd3(sinK, cosK2));

	return (R);
	// std::cout << " ---- " << std::endl;
	// for (int y = 0; y < 3; y++) {
	// 	for (int x = 0; x < 3; x++) {
	// 		std::cout << R.data[y][x] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;


	// std::cout << angle << std::endl;
	// std::cout << NrotAxis.x << " " << NrotAxis.y << " " << NrotAxis.z << std::endl;
}

mat3 matMult3Float(mat3 mat, float num) {
	mat3 matrix;

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			matrix.data[y][x] = mat.data[y][x] * num;
		}
	}

	return (matrix);
}

mat3 matadd3(mat3 a, mat3 b) {
	mat3 matrix;

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			matrix.data[y][x] = a.data[y][x] + b.data[y][x];
		}
	}

	return (matrix);
}

mat3 matMult3(mat3 a, mat3 b) {
	mat3	matrix;

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			matrix.data[y][x] = a.data[y][0] * b.data[0][x] + a.data[y][1] * b.data[1][x] + a.data[y][2] * b.data[2][x];
		}
	}

	return (matrix);
}

mat3 identityMat3(float id) {
	mat3	matrix;

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			matrix.data[y][x] = 0;
		}
	}

	matrix.data[0][0] = id;
	matrix.data[1][1] = id;
	matrix.data[2][2] = id;

	return (matrix);
}

void populateMat(float buf[16], mat4 m)
{
	for (int y = 0; y < 4; ++y)
		for (int x = 0; x < 4; ++x)
			buf[4 * y + x] = m.data[x][y];
}

float vecDot(vec3 a, vec3 b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
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

void populateVec4(float buf[4], vec4 v) {
	buf[0] = v.x;
	buf[1] = v.y;
	buf[2] = v.z;
	buf[3] = v.w;
}