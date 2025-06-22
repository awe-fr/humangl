#include "./../includes/GraphicsMath.h"

#include <iostream>
float det2(float a, float b, float c, float d) {
    return (a * d - c * b);
}

float det3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
    float d1 = det2(e, f, h, i);
    float d2 = det2(d, f, g, i);
    float d3 = det2(d, e, g, h);

    return (a * d1 - b * d2 + c * d3);
}

mat4 matComplementary(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
	mat4 matrix = identityMat(1);
	matrix.data[0][0] = det2(e, f, h, i);
	matrix.data[0][1] = det2(d, f, g, i);
	matrix.data[0][2] = det2(d, e, g, h);

	matrix.data[1][0] = det2(b, c, h, i);
	matrix.data[1][1] = det2(a, c, g, i);
	matrix.data[1][2] = det2(a, b, g, h);

	matrix.data[2][0] = det2(b, c, e, f);
	matrix.data[2][1] = det2(a, c, d, f);
	matrix.data[2][2] = det2(a, b, d, e);

	return (matrix);
}

mat4 comatrix(mat4 a) {
	a.data[0][1] *= -1;

	a.data[1][0] *= -1;
	a.data[1][2] *= -1;

	a.data[2][1] *= -1;

	return (a);
} 

mat4 matInverse(mat4 a) {
	float matDet = det3(a.data[0][0], a.data[0][1], a.data[0][2], a.data[1][0], a.data[1][1], a.data[1][2], a.data[2][0], a.data[2][1], a.data[2][2]);
	mat4 matCompl = matComplementary(a.data[0][0], a.data[0][1], a.data[0][2], a.data[1][0], a.data[1][1], a.data[1][2], a.data[2][0], a.data[2][1], a.data[2][2]);
	mat4 coMat = comatrix(matCompl);
	mat4 matCompose = transposemat(coMat);

	matCompose.data[0][0] *= (1/matDet);
	matCompose.data[0][1] *= (1/matDet);
	matCompose.data[0][2] *= (1/matDet);

	matCompose.data[1][0] *= (1/matDet);
	matCompose.data[1][1] *= (1/matDet);
	matCompose.data[1][2] *= (1/matDet);

	matCompose.data[2][0] *= (1/matDet);
	matCompose.data[2][1] *= (1/matDet);
	matCompose.data[2][2] *= (1/matDet);

	// printMat(coMat);
	// printMat(matCompose);
	
	return (matCompose); //cahnge return
}

quat quatNormalize(quat q) {
    float length = sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    if (length == 0.0f) return quat{1.0f, 0.0f, 0.0f, 0.0f}; // Par sécurité, renvoyer un quaternion identité si la norme est zéro
    return quat{q.w / length, q.x / length, q.y / length, q.z / length};
}

mat3 quatToMat(quat q) {
	mat3 mat;

	mat.data[0][0] = 1.0f - 2.0f * (q.y * q.y) - 2.0f * (q.z * q.z);
	mat.data[0][1] = 2.0f * q.x * q.y - 2.0f * q.z * q.w;
	mat.data[0][2] = 2.0f * q.x * q.z + 2.0f * q.y * q.w;

	mat.data[1][0] = 2 * q.x * q.y + 2.0f * q.z * q.w;
	mat.data[1][1] = 1.0f - 2.0f * (q.x * q.x) - 2.0f * (q.z * q.z);
	mat.data[1][2] = 2.0f * q.y * q.z - 2.0f * q.x * q.w;

	mat.data[2][0] = 2.0f * q.x * q.z - 2.0f * q.y * q.w;
	mat.data[2][1] = 2.0f * q.y * q.z + 2.0f * q.x * q.w;
	mat.data[2][2] = 1.0f - 2.0f * (q.x * q.x) - 2.0f * (q.y * q.y);

	return(mat);
}

quat eulerToQuat(float roll, float pitch, float yaw) {
    float cy = cos(yaw * 0.5f);
    float sy = sin(yaw * 0.5f);
    float cp = cos(pitch * 0.5f);
    float sp = sin(pitch * 0.5f);
    float cr = cos(roll * 0.5f);
    float sr = sin(roll * 0.5f);

	quat ret;
    ret.w = cr * cp * cy + sr * sp * sy;
    ret.x = sr * cp * cy - cr * sp * sy;
    ret.y = cr * sp * cy + sr * cp * sy;
    ret.z = cr * cp * sy - sr * sp * cy;

    return (ret);
}

quat quatMult(quat a, quat b) {
	quat ret;
	ret.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;  // 1
    ret.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;  // i
    ret.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;  // j
    ret.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;

	return (quatNormalize(ret));
}

quat quatMat2(vec3 dir, vec3 orr) {
    // Normalize the direction and up vectors
	quat ret;
    vec3 Ndir = vecNormalize(dir);
    vec3 Nup = vecNormalize(orr);
    
    // Compute the axis of rotation (cross product of dir and up)
    vec3 NrotAxis = vecCross(Nup, Ndir);
    
    // If the vectors are parallel (cross product is zero), return the identity quaternion
    if (NrotAxis.x == 0 && NrotAxis.y == 0 && NrotAxis.z == 0) {
		ret.w = 1; ret.x = 0; ret.y = 0; ret.z = 0;
        return (ret); // Identity quaternion (no rotation)
    }
    
    // Normalize the axis of rotation
    NrotAxis = vecNormalize(NrotAxis);
    
    // Calculate the angle between the two vectors (dir and up)
    float angle = acos(vecDot(Nup, Ndir));
    
    // Half-angle
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = sin(halfAngle);
    float cosHalfAngle = cos(halfAngle);
    
    // Construct the quaternion from the axis of rotation and angle
	ret.w = cosHalfAngle;
	ret.x = NrotAxis.x * sinHalfAngle;
	ret.y = NrotAxis.y * sinHalfAngle;
	ret.z = NrotAxis.z * sinHalfAngle;
    
    return (ret);
}






void printMat(mat4 m) {
	for (int i = 0; i < 4; i++) {
		for (int y = 0; y < 4; y++) {
			std::cout << m.data[i][y] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

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

mat3 quatMat(vec3 dir, vec3 orr) {
	vec3 Ndir = vecNormalize(dir);
	vec3 Nup = vecNormalize(orr);
	vec3 NrotAxis = vecCross(Nup, Ndir);
	if (NrotAxis.x != 0 || NrotAxis.y != 0 || NrotAxis.z != 0)
		NrotAxis = vecNormalize(NrotAxis);
	else
		return (identityMat3(1));
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


	mat3 K2 = matMult3(K, K);

	mat3 sinK = matMult3Float(K, Sangle);
	mat3 cosK2 = matMult3Float(K2, (1 - Cangle));

	mat3 R = matadd3(identityMat3(1), matadd3(sinK, cosK2));

	return (R);
}

mat4 transposemat(mat4 to_inv) {
	mat4 matrix = to_inv;

	for (int i = 0; i < 3; i++) {
		for (int y = 0; y < 3; y++) {
			matrix.data[i][y] = to_inv.data[y][i];
		}
	}

	return (matrix);
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

	float a = sqrtf((f.x * f.x) + (f.y * f.y) + (f.z * f.z));

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