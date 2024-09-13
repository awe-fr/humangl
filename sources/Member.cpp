#include "../includes/Member.hpp"
#include "../includes/ImguiValues.hpp"

Member::Member(std::string name, vec3 direction, float length, vec3 degree, std::map<std::string, Limit> degree_lock, Member *previous, Root *root) :
				_name(name), _direction(direction), _length(length), _base_length(length), _degree(degree), _degreeLock(degree_lock), _previous(previous), _root(root)
{
	this->_model = identityMat(1);

	this->_vertex = new GLfloat[sizeof(GLfloat) * 24];
	this->_vertex[0] = MEMBER_BASE_WIDTH_END;		this->_vertex[1] = length;  this->_vertex[2] = MEMBER_BASE_WIDTH_END;
	this->_vertex[3] = -MEMBER_BASE_WIDTH_END;		this->_vertex[4] = length;  this->_vertex[5] = -MEMBER_BASE_WIDTH_END;
	this->_vertex[6] = -MEMBER_BASE_WIDTH_END;		this->_vertex[7] = length;  this->_vertex[8] = MEMBER_BASE_WIDTH_END;
	this->_vertex[9] = MEMBER_BASE_WIDTH_START;		this->_vertex[10] = 0;      this->_vertex[11] = -MEMBER_BASE_WIDTH_START;
	this->_vertex[12] = -MEMBER_BASE_WIDTH_START;	this->_vertex[13] = 0;      this->_vertex[14] = -MEMBER_BASE_WIDTH_START;
	this->_vertex[15] = MEMBER_BASE_WIDTH_END;   	this->_vertex[16] = length; this->_vertex[17] = -MEMBER_BASE_WIDTH_END;
	this->_vertex[18] = MEMBER_BASE_WIDTH_START;	this->_vertex[19] = 0;      this->_vertex[20] = MEMBER_BASE_WIDTH_START;
	this->_vertex[21] = -MEMBER_BASE_WIDTH_START;	this->_vertex[22] = 0;      this->_vertex[23] = MEMBER_BASE_WIDTH_START;

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

mat4 inverse(mat4 m);

void printMat(mat4 m) {
	for (int i = 0; i < 4; i++) {
		for (int y = 0; y < 4; y++) {
			std::cout << m.data[i][y] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Member::computeTravel() {
	this->_model = identityMat(1);

	if (this->_previous != nullptr) {
		this->_model = this->_previous->_model;
		// mat4 prev = translationMat(this->_direction.x * this->_length, 
		//  													this->_direction.y * this->_length, 
		//  													this->_direction.z * this->_length);
		// this->_model = this->_previous->_model;
		// if (this->_previous->_previous == nullptr)
		// 	this->_model = identityMat(1);
		// this->_model = matMult(this->_model, translationMat(this->_previous->_direction.x * this->_previous->_length, 
		// 													this->_previous->_direction.y * this->_previous->_length, 
		// 													this->_previous->_direction.z * this->_previous->_length));
		// this->_model = matMult(this->_model,quatMat(this->_direction));
		this->_model = matMult(this->_model, translationMat(0, this->_previous->_length, 0));
		// std::cout << this->_name << std::endl;
		this->_model = matMult(this->_model, upcastmat3(quatMat(this->_direction, this->_previous->_direction)));
		// std::cout << this->_previous->_direction.x << " " << this->_previous->_direction.y << " " << this->_previous->_direction.z << std::endl;
	}
	else if (this->_root != nullptr) {
		this->_model = upcastmat3(quatMat(this->_direction, {0,1,0}));
		// this->_model = this->_root->getModel();
		// this->_model = directionMat(this->_direction);
		// this->_model = matMult(this->_model, translationMat(this->_direction.x * this->_length, 
		// 													this->_direction.y * this->_length, 
		// 													this->_direction.z * this->_length));
		// this->_model = matMult(this->_model, translationMat(0, this->_previous->_length, 0));
	}
	if (this->_previous != nullptr)
		std::cout << this->_name << " : " << this->_direction.x << " " << this->_direction.y << " " << this->_direction.z << std::endl;
	else 
		std::cout << this->_name << std::endl;
	// printMat(this->_model);
	// printMat(inverse(this->_model));
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

void Member::update(void *param)
{
	Param *ratio = (Param *)param;

	switch (ratio->type)
	{
		case Length:
			this->_length = ratio->value * this->_base_length;

			this->_vertex[1] = this->_length;
			this->_vertex[4] = this->_length;
			this->_vertex[7] = this->_length;
			this->_vertex[16] = this->_length;
			break;

		case Width:
			this->_vertex[9] = ratio->value * MEMBER_BASE_WIDTH_START;
			this->_vertex[12] = ratio->value * (-MEMBER_BASE_WIDTH_START);
			this->_vertex[18] = ratio->value * MEMBER_BASE_WIDTH_START;
			this->_vertex[21] = ratio->value * (-MEMBER_BASE_WIDTH_START);
			this->_vertex[11] = ratio->value * (-MEMBER_BASE_WIDTH_START);
			this->_vertex[14] = ratio->value * (-MEMBER_BASE_WIDTH_START);
			this->_vertex[20] = ratio->value * MEMBER_BASE_WIDTH_START;
			this->_vertex[23] = ratio->value * MEMBER_BASE_WIDTH_START;

			this->_vertex[0] = ratio->value * MEMBER_BASE_WIDTH_END;
			this->_vertex[3] = ratio->value * (-MEMBER_BASE_WIDTH_END);
			this->_vertex[6] = ratio->value * (-MEMBER_BASE_WIDTH_END);
			this->_vertex[15] = ratio->value * MEMBER_BASE_WIDTH_END;
			this->_vertex[2] = ratio->value * MEMBER_BASE_WIDTH_END;
			this->_vertex[5] = ratio->value * (-MEMBER_BASE_WIDTH_END);
			this->_vertex[8] = ratio->value * MEMBER_BASE_WIDTH_END;
			this->_vertex[17] = ratio->value * (-MEMBER_BASE_WIDTH_END);
			break;
		
		default:
			break;
	}

	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, &this->_vertex[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}