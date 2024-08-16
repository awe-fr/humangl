#ifndef MATH_H
# define MATH_H

struct mat4 {
	float data[4][4];	
};

struct vec4 {
	float x;
	float y;
	float z;
	float w;
};

mat4 matMult(mat4 a, mat4 b);
mat4 identityMat(float id);
mat4 scaletyMat(float x, float y, float z);

#endif