#include "./../includes/Member.hpp"

Member::Member(std::string name, float length) {
	// this->_next = nullptr;
	this->_previous = nullptr;
	this->_name = name;
	this->_length = length;
	this->_degreeLock.x = 0; this->_degreeLock.y = 0; this->_degreeLock.z = 0;

	this->_model = identityMat(1);
	this->_degree.x = 0; this->_degree.y = 0; this->_degree.z = 0;

	this->_vertex = new GLfloat[sizeof(GLfloat) * 24];
	this->_vertex[0] = 0.0f;    this->_vertex[1] = length;  this->_vertex[2] = 0.0f;
	this->_vertex[3] = -0.0f;   this->_vertex[4] = length;  this->_vertex[5] = -0.0f;
	this->_vertex[6] = -0.0f;   this->_vertex[7] = length;  this->_vertex[8] = 0.0f;
	this->_vertex[9] = 0.05f;   this->_vertex[10] = 0;      this->_vertex[11] = -0.05f;
	this->_vertex[12] = -0.05f; this->_vertex[13] = 0;      this->_vertex[14] = -0.05f;
	this->_vertex[15] = 0.0f;   this->_vertex[16] = length; this->_vertex[17] = -0.0f;
	this->_vertex[18] = 0.05f;  this->_vertex[19] = 0;      this->_vertex[20] = 0.05f;
	this->_vertex[21] = -0.05f; this->_vertex[22] = 0;      this->_vertex[23] = 0.05f;

	this->_index = new int[sizeof(int) * 36];
	this->_index[0] = 0;  this->_index[1] = 1;  this->_index[2] = 2;
	this->_index[3] = 1;  this->_index[4] = 3;  this->_index[5] = 4;
	this->_index[6] = 5;  this->_index[7] = 6;  this->_index[8] = 3;
	this->_index[9] = 7;  this->_index[10] = 3; this->_index[11] = 6;
	this->_index[12] = 2; this->_index[13] = 4; this->_index[14] = 7;
	this->_index[15] = 0; this->_index[16] = 7; this->_index[17] = 6;
	this->_index[18] = 0; this->_index[19] = 5; this->_index[20] = 1;
	this->_index[21] = 1; this->_index[22] = 5; this->_index[23] = 3;
	this->_index[24] = 5; this->_index[25] = 0; this->_index[26] = 6;
	this->_index[27] = 7; this->_index[28] = 4; this->_index[29] = 3;
	this->_index[30] = 2; this->_index[31] = 1; this->_index[32] = 4;
	this->_index[33] = 0; this->_index[34] = 2; this->_index[35] = 7;

	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);

	glGenBuffers(1, &this->_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, &this->_vertex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 36, &this->_index[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Member::~Member() {
	glDeleteBuffers(1, &this->_vbo);
	glDeleteBuffers(1, &this->_ibo);
	glDeleteVertexArrays(1, &this->_vao);

	delete[] this->_vertex;
	delete[] this->_index;
}

void Member::setDegree(float x, float y, float z) {
	this->_degree.x = x;
	this->_degree.y = y;
	this->_degree.z = z;
}

void Member::setHistory() {
	Member *temp = this;
	this->_degreeStack.push_back(temp->_degree);
	while(temp->_previous != nullptr) {
		temp = temp->_previous;
		this->_degreeStack.push_back(temp->_degree);
	}
}

void Member::computeTravel() {
	this->_model = identityMat(1);
	if (this->_previous != nullptr) {
		this->_model = this->_previous->_model;
		this->_model = matMult(this->_model, translationMat(0, 1, 0));
	}
	this->_model = matMult(this->_model, rotationMatX(this->_degree.x));
	this->_model = matMult(this->_model, rotationMatY(this->_degree.y));
	this->_model = matMult(this->_model, rotationMatZ(this->_degree.z));
}

void Member::printHistory() {
	for(int i = 0; i < this->_degreeStack.size(); i++) {
		std::cout << this->_name << " : " << this->_degreeStack[i].x << ", " << this->_degreeStack[i].y << ", " << this->_degreeStack[i].z << std::endl;
	}
}

void Member::clearStack() {
	this->_degreeStack.clear();
}

// void Member::addNext(Member *n) {
// 	this->_next = n;
// }

void Member::addPrevious(Member *p) {
	this->_previous = p;
}

GLuint Member::getVAO() {
	return (this->_vao);
}

GLuint Member::getVBO() {
	return (this->_vbo);
}

GLuint Member::getIBO() {
	return (this->_ibo);
}

mat4 Member::getModel() {
	return (this->_model);
}