#ifndef GRAPHICSMATH_H
# define GRAPHICSMATH_H

# include "./header.h"

struct mat4 {
	float data[4][4];	
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

mat4 matMult(mat4 a, mat4 b);
mat4 identityMat(float id);
mat4 scaletyMat(float x, float y, float z);
mat4 projectionMat(float FOV, float aspect, float znear, float zfar);
mat4 viewMat(vec3 eye, vec3 target, vec3 up);
void populateMat(float buf[16], mat4 m);

vec3 vecCross(vec3 f, vec3 d);
vec3 vecNormalize(vec3 f);
vec3 vecMultiplication(vec3 f, float s);
vec3 vecAddition(vec3 f, vec3 s);
vec3 vecSubstract(vec3 f, vec3 s);

#endif