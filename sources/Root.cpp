#include "../includes/Root.hpp"

Root::Root(std::vector<std::string> order) : _order(order) {
	this->_model = identityMat(1);
	
	this->_vertex = new GLfloat[sizeof(GLfloat) * 24];
	this->_vertex[0] = 0.2f;    this->_vertex[1] = 0.2;  this->_vertex[2] = 0.2f;
	this->_vertex[3] = -0.2f;   this->_vertex[4] = 0.2;  this->_vertex[5] = -0.2f;
	this->_vertex[6] = -0.2f;   this->_vertex[7] = 0.2;  this->_vertex[8] = 0.2f;
	this->_vertex[9] = 0.2f;   this->_vertex[10] = -0.2; this->_vertex[11] = -0.2f;
	this->_vertex[12] = -0.2f; this->_vertex[13] = -0.2; this->_vertex[14] = -0.2f;
	this->_vertex[15] = 0.2f;   this->_vertex[16] = 0.2; this->_vertex[17] = -0.2f;
	this->_vertex[18] = 0.2f;  this->_vertex[19] = -0.2; this->_vertex[20] = 0.2f;
	this->_vertex[21] = -0.2f; this->_vertex[22] = -0.2; this->_vertex[23] = 0.2f;
	
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
};

Root::~Root(void) {
    glDeleteBuffers(1, &this->_vbo);
	glDeleteBuffers(1, &this->_ibo);
	glDeleteVertexArrays(1, &this->_vao);

	delete[] this->_vertex;
	delete[] this->_index;
}

GLuint Root::getVAO() {
	return (this->_vao);
}

GLuint Root::getVBO() {
	return (this->_vbo);
}

GLuint Root::getIBO() {
	return (this->_ibo);
}

mat4 Root::getModel() {
	return (this->_model);
}

void Root::setPosition(vec3 new_position)
{
	this->_postition = new_position;
}

void Root::setOrientation(vec3 new_orientation)
{
	this->_orientation = new_orientation;
}