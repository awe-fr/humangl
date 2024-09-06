#include "../includes/Member.hpp"

Member::Member(std::string name, vec3 direction, float length, vec3 degree, std::map<std::string, Limit> degree_lock, Member *previous, Root *root) :
				_name(name), _direction(direction), _length(length), _degree(degree), _degreeLock(degree_lock), _previous(previous), _root(root)
{
	this->_model = identityMat(1);

	this->_vertex = new GLfloat[sizeof(GLfloat) * 24];
	this->_vertex[0] = 0.01f;    this->_vertex[1] = length;  this->_vertex[2] = 0.01f;
	this->_vertex[3] = -0.01f;   this->_vertex[4] = length;  this->_vertex[5] = -0.01f;
	this->_vertex[6] = -0.01f;   this->_vertex[7] = length;  this->_vertex[8] = 0.01f;
	this->_vertex[9] = 0.05f;   this->_vertex[10] = 0;      this->_vertex[11] = -0.05f;
	this->_vertex[12] = -0.05f; this->_vertex[13] = 0;      this->_vertex[14] = -0.05f;
	this->_vertex[15] = 0.01f;   this->_vertex[16] = length; this->_vertex[17] = -0.01f;
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

void Member::computeTravel() {
	this->_model = identityMat(1);
	if (this->_name == "lhipjoint") {
		this->_model.data[0][0] = -0.5001;
		this->_model.data[0][1] = 0.6556;
		this->_model.data[0][2] = -0.5657;
		this->_model.data[1][0] = -0.6556;
		this->_model.data[1][1] = -0.7134;
		this->_model.data[1][2] = -0.2472;
		this->_model.data[2][0] = -0.5657;
		this->_model.data[2][1] = 0.2472;
		this->_model.data[2][2] = 0.7867;
	}
	else if (this->_name == "rhipjoint"){
		this->_model.data[0][0] = -0.4829;
		this->_model.data[0][1] = -0.6451;
		this->_model.data[0][2] = -0.5912;
		this->_model.data[1][0] = 0.6451;
		this->_model.data[1][1] = -0.7220;
		this->_model.data[1][2] = 0.2502;
		this->_model.data[2][0] = 0.5912;
		this->_model.data[2][1] = 0.2502;
		this->_model.data[2][2] = -0.7677;
	}
	else {
		this->_model.data[0][0] = -0.9998186;
		this->_model.data[0][1] = 0.01898626;
		this->_model.data[0][2] = -0.00130494;
		this->_model.data[1][0] = 0.01898626;
		this->_model.data[1][1] = 0.990426032;
		this->_model.data[1][2] = -0.13671326;
		this->_model.data[2][0] = -0.00130494;
		this->_model.data[2][1] = -0.13671326;
		this->_model.data[2][2] = -0.99060936;
	}
	if (this->_previous != nullptr) {
		// mat4 prev = translationMat(this->_direction.x * this->_length, 
		//  													this->_direction.y * this->_length, 
		//  													this->_direction.z * this->_length);
		// this->_model = this->_previous->_model;
		// this->_model = matMult(this->_model, translationMat(this->_previous->_direction.x * this->_previous->_length, 
		// 													this->_previous->_direction.y * this->_previous->_length, 
		// 													this->_previous->_direction.z * this->_previous->_length));
		// this->_model = matMult(this->_model, translationMat(0, this->_previous->_length, 0));
	}
	else if (this->_root != nullptr) {
		// this->_model = this->_root->getModel();
		// this->_model = directionMat(this->_direction);
		// this->_model = matMult(this->_model, translationMat(this->_direction.x * this->_length, 
		// 													this->_direction.y * this->_length, 
		// 													this->_direction.z * this->_length));
		// this->_model = matMult(this->_model, translationMat(0, this->_previous->_length, 0));
	}
	// this->_model = matMult(this->_model, rotationMatX(this->_degree.x));
	// this->_model = matMult(this->_model, rotationMatY(this->_degree.y));
	// this->_model = matMult(this->_model, rotationMatZ(this->_degree.z));
}

void Member::setPrevious(Member *p) {
	this->_previous = p;
}

void Member::setDegreeX(float x)
{
	this->_degree.x = x;
}

void Member::setDegreeY(float y)
{
	this->_degree.y = y;
}

void Member::setDegreeZ(float z)
{
	this->_degree.z = z;
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

std::string Member::getName() {
	return (this->_name);
}

std::map<std::string, Limit> &Member::getDegreeLock(void)
{
	return this->_degreeLock;
}

void Member::printName() {
	if (this->_previous != nullptr)
		std::cout << this->_name << " : " << this->_previous->_name << std::endl;
	else
		std::cout << this->_name << ": Root" << std::endl;
}