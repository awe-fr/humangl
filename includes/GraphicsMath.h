#ifndef GRAPHICSMATH_H
# define GRAPHICSMATH_H

# include <cmath>

# define DEG2RAD acos(-1.0f) / 180.0f

struct mat4 {
	float data[4][4];	
};

struct mat3 {
	float data[3][3];	
};

struct vec4 {
	float x;
	float y;
	float z;
	float w;
};

struct vec3 {
	float x;
	float y;
	float z;
};

mat4 directionMat(vec3 direction);
mat4 matMult(mat4 a, mat4 b);
mat4 identityMat(float id);
mat4 scaletyMat(float x, float y, float z);
mat4 projectionMat(float FOV, float aspect, float znear, float zfar);
mat4 viewMat(vec3 eye, vec3 target, vec3 up);
mat4 translationMat(float x, float y, float z);
mat4 rotationMatY(float degree);
mat4 rotationMatX(float degree);
mat4 rotationMatZ(float degree);
mat4 upcastmat3(mat3 toUp);
void populateMat(float buf[16], mat4 m);

mat3 quatMat(vec3 dir, vec3 orr);
mat3 matMult3Float(mat3 mat, float num);
mat3 matMult3(mat3 a, mat3 b);
mat3 matadd3(mat3 a, mat3 b);
mat3 identityMat3(float id);

float vecDot(vec3 a, vec3 b);
vec3 vecCross(vec3 f, vec3 d);
vec3 vecNormalize(vec3 f);
vec3 vecMultiplication(vec3 f, float s);
vec3 vecAddition(vec3 f, vec3 s);
vec3 vecSubstract(vec3 f, vec3 s);
void populateVec4(float buf[4], vec4 v);

#endif