#include "../includes/Member.hpp"
#include "../includes/ImguiValues.hpp"

Member::Member(std::string name, vec3 direction, float length, vec3 degree, Member *previous, Root *root) :
				_name(name), _direction(direction), _length(length), _base_length(length), _degree(degree), _previous(previous), _root(root)
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
		// Member *tmp = this->_previous;
		// while(tmp != nullptr) {
		// 	this->_model = matMult(this->_model, translationMat(tmp->_direction.x * tmp->_length, 
		// 														tmp->_direction.y * tmp->_length, 
		// 														tmp->_direction.z * tmp->_length));
		// 	tmp = tmp->_previous;
		// }
		// vec3 tmp = this->_previous->_direction;
		// this->_model = matMult(this->_model,upcastmat3(quatMat(this->_direction, {0,1,0})));
		this->_model = matMult(this->_model, translationMat(0, this->_previous->_length, 0));
		
		mat4 rev = matInverse(this->_previous->_model);
		this->_model = matMult(this->_model, rev);

		// quat dir = quatMat2({this->_direction.x,this->_direction.y,this->_direction.z}, {0,1,0});
		// quat rot = eulerToQuat(this->_degree.x * DEG2RAD, this->_degree.y * DEG2RAD, this->_degree.z * DEG2RAD);
		// quat final = quatMult(rot, dir);
		
		// quat final = quatMult(dir, rot);
		// quat final = quatNormalize(dir);
		// this->_model = matMult(this->_model, upcastmat3(quatToMat(final)));
		quat rot;
		// if (this->_name == "lfemur") {
		// 	rot = eulerToQuat(-30 * DEG2RAD, -14 * DEG2RAD, -20 * DEG2RAD);
		// }
		// else if (this->_name == "ltibia") {
		// 	rot = eulerToQuat((-30 + 70.445518) * DEG2RAD, (-14 + 18.930830) * DEG2RAD, (-20 + 13.426644) * DEG2RAD);
		// }
		// else if (this->_name == "lfoot") {
		// 	rot = eulerToQuat(((-30 + 70.445518) + -9.708598) * DEG2RAD, ((-14 + 18.930830) + 7.324748) * DEG2RAD, ((-20 + 13.426644) + -0.031136) * DEG2RAD);
		// }			
		// else if (this->_name == "ltoes") {
		// 	rot = eulerToQuat((((-30 + 70.445518) + -9.708598) + -10.198438) * DEG2RAD, (((-14 + 18.930830) + 7.324748) + -3.687259) * DEG2RAD, (((-20 + 13.426644) + -0.031136) + 0.329141) * DEG2RAD);
		// }
		// else if (this->_name == "rfemur") {
		// 	rot = eulerToQuat(-2.5 * DEG2RAD, 1 * DEG2RAD, 26.673158 * DEG2RAD);
		// }
		// else if (this->_name == "rtibia") {
		// 	rot = eulerToQuat((-2.5 + 28.370413) * DEG2RAD, (1 + -9.812113) * DEG2RAD, (26.673158 + -2.485840) * DEG2RAD);
		// }	
		// else if (this->_name == "rfoot") {
		// 	rot = eulerToQuat(((-2.5 + 28.370413) + -24.318613) * DEG2RAD, ((1 + -9.812113) + 1.343554) * DEG2RAD, ((26.673158 + -2.485840) + -1.705908) * DEG2RAD);
		// }		
		// else if (this->_name == "rtoes") {
		// 	rot = eulerToQuat((((-2.5 + 28.370413) + -24.318613) + 5.195411) * DEG2RAD, (((1 + -9.812113) + 1.343554) + -1.887701) * DEG2RAD, (((26.673158 + -2.485840) + -1.705908) + -0.085653) * DEG2RAD);
		// }
		// else if (this->_name == "lowerback") {
		// 	rot = eulerToQuat((10.059501) * DEG2RAD, (-0.098741) * DEG2RAD, (5.906470) * DEG2RAD);
		// }
		// else if (this->_name == "upperback") {
		// 	rot = eulerToQuat((10.059501 + 1.159330) * DEG2RAD, (-0.098741 + 0.561044) * DEG2RAD, (5.906470 + 0.897338) * DEG2RAD);
		// }	
		// else if (this->_name == "thorax") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595) * DEG2RAD, (5.906470 + 0.897338 + -2.464480) * DEG2RAD);
		// }
		// else if (this->_name == "lowerneck") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + -18.144499) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 1.124480) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130) * DEG2RAD);
		// }			
		// else if (this->_name == "upperneck") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + -18.144499 + -0.962868) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 1.124480 + 2.242470) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + 1.449120) * DEG2RAD);
		// }			
		// else if (this->_name == "head") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + -18.144499 + -0.962868 + 2.804640) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 1.124480 + 2.242470 + 1.170480) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + 1.449120 + 1.377690) * DEG2RAD);
		// }			
		// else if (this->_name == "lclavicle") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130) * DEG2RAD);
		// }
		// else if (this->_name == "lhumerus") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 30.832748) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 16.238470) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + -87.849632) * DEG2RAD);
		// }
		// else if (this->_name == "lradius") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 30.832748 + -6.328650) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 16.238470 + -32.434898) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + -87.849632 + 21.524012) * DEG2RAD);
		// }			
		// else if (this->_name == "lwrist") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 30.832748 + -6.328650 + -25.993092) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 16.238470 + -32.434898 + 0) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + -87.849632 + 21.524012 + 0) * DEG2RAD);
		// }	
		// else if (this->_name == "lhand") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 30.832748 + -6.328650 + -25.993092 + 10.289421) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 16.238470 + -32.434898 + 0 + 21.866867) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + -87.849632 + 21.524012 + 0 + 25.985512) * DEG2RAD);
		// }
		// else if (this->_name == "lfingers") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 30.832748 + -6.328650 + -25.993092 + 10.289421 + 0) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 16.238470 + -32.434898 + 0 + 21.866867 + 0) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + -87.849632 + 21.524012 + 0 + 25.985512 + -7.125019) * DEG2RAD);
		// }
		// else if (this->_name == "lthumb") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 30.832748 + -6.328650 + -25.993092 + 10.289421 + 0 + -0.662982) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 16.238470 + -32.434898 + 0 + 21.866867 + 0 + 54.013943) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + -87.849632 + 21.524012 + 0 + 25.985512 + -7.125019 + -2.990124) * DEG2RAD);
		// }			
		// else if (this->_name == "rclavicle") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130) * DEG2RAD);
		// }
		// else if (this->_name == "rhumerus") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 39.894665) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 5.542181) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + 85.453896) * DEG2RAD);
		// }
		// else if (this->_name == "rradius") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 39.894665 + -15.058424) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 5.542181 + 45.058292) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + 85.453896 + -35.346966) * DEG2RAD);
		// }			
		// else if (this->_name == "rwrist") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 39.894665 + -15.058424 + -22.218000) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 5.542181 + 45.058292 + 0) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + 85.453896 + -35.346966 + 0) * DEG2RAD);
		// }
		// else if (this->_name == "lhand") {
		// 	rot = eulerToQuat((10.059501 + 1.159330 + -4.387101 + 0 + 39.894665 + -15.058424 + -22.218000 + -12.386736) * DEG2RAD, (-0.098741 + 0.561044 + 0.181595 + 0 + 5.542181 + 45.058292 + 0 + 19.039490) * DEG2RAD, (5.906470 + 0.897338 + -2.464480 + -4.017130 + 85.453896 + -35.346966 + 0 + -33.949699) * DEG2RAD);
		// }			
		// else if (this->_name == "lfingers") {
		// 	rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + 46.967899 + -17.181623 + -21.534393 + -11.491422 + 0) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 18.733820 + 47.027115 + 0 + 18.305342 + 0) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + 78.647438 + -38.295696 + 0 + -32.913929 + 7.125019) * DEG2RAD);
		// }
		// else if (this->_name == "rthumb") {
		// 	rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + 46.967899 + -17.181623 + -21.534393 + -11.491422 + 0 + -3.151534) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 18.733820 + 47.027115 + 0 + 18.305342 + 0 + -7.429551) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + 78.647438 + -38.295696 + 0 + -32.913929 + 7.125019 + -3.647205) * DEG2RAD);
		// }		
		// else
		if (this->_previous != nullptr) {
			this->_degree.z += this->_previous->_degree.z;
			this->_degree.y += this->_previous->_degree.y;
			this->_degree.x += this->_previous->_degree.x;
			rot = eulerToQuat((this->_degree.z) * DEG2RAD, (this->_degree.y) * DEG2RAD, (this->_degree.x) * DEG2RAD);
		}
		rot = eulerToQuat(this->_degree.z * DEG2RAD, this->_degree.y * DEG2RAD, this->_degree.x * DEG2RAD);

		// if (this->_name == "ltibia") {
		// 	std::cout << this->_degree.x << " " << this->_degree.y << " " << this->_degree.z << std::endl;
		// }

		// if (this->_name == "lfemur" || this->_name == "ltibia" ) {
		// 	if (this->_name == "lfemur")
		// 		this->_model = matMult(this->_model, upcastmat3(quatToMat(eulerToQuat(-39.022781 * DEG2RAD, 0, 20 * DEG2RAD))));
		// 	else
		// 		this->_model = matMult(this->_model, upcastmat3(quatToMat(eulerToQuat(0, 0, 20 * DEG2RAD))));
		// 	std::cout << this->_degree.x << ", " << this->_degree.y << ", " << this->_degree.z << std::endl;
		// 	quat dir = quatMat2({this->_direction.x,this->_direction.y,this->_direction.z}, {0,1,0});
		// 	quat rot = eulerToQuat(this->_degree.x * DEG2RAD, this->_degree.y * DEG2RAD, (this->_degree.z) * DEG2RAD);
		// 	quat final = quatMult(rot, dir);
		// 	this->_model = matMult(this->_model, upcastmat3(quatToMat(final)));
		// }
		// else {
			quat dir = quatMat2({this->_direction.x,this->_direction.y,this->_direction.z}, {0,1,0});
			// quat rot = eulerToQuat(this->_degree.x * DEG2RAD, this->_degree.y * DEG2RAD, this->_degree.z * DEG2RAD);
			quat final = quatMult(rot, dir);
			this->_model = matMult(this->_model, upcastmat3(quatToMat(final)));
		// }
		// 	mat4 test = identityMat(1);
		// 	test = matMult(test, rotationMatZ(this->_degree.z));
		// 	test = matMult(test, rotationMatY(this->_degree.y));
		// 	test = matMult(test, rotationMatX(this->_degree.x));

		// 	this->_model = matMult(this->_model, test);
		// }
		// else if (this->_name == "ltibia") {
		// }
		// else if (this->_name == "lfoot") {
		// }
		// else if (this->_name == "ltoes") {
		// }
		// std::cout << this->_name << std::endl;
		// this->_model = matMult(this->_model, transposemat(this->_previous->_model));
		// tmp.x = -tmp.x;
		// tmp.y = -tmp.y;
		// std::cout << this->_previous->_direction.x << " " << this->_previous->_direction.y << " " << this->_previous->_direction.z << std::endl;
		// this->_model = matMult(this->_model, upcastmat3(quatMat(this->_direction, tmp)));
	}
	else if (this->_root != nullptr) {
		this->_model = upcastmat3(quatMat(this->_direction, {0,1,0}));
		return;
		// this->_model = this->_root->getModel();
		// this->_model = directionMat(this->_direction);
		// this->_model = matMult(this->_model, translationMat(this->_direction.x * this->_length, 
		// 													this->_direction.y * this->_length, 
		// 													this->_direction.z * this->_length));
		// this->_model = matMult(this->_model, translationMat(0, this->_previous->_length, 0));
	}


	// if (this->_previous != nullptr)
	// 	std::cout << this->_name << " : " << this->_direction.x << " " << this->_direction.y << " " << this->_direction.z << std::endl;
	// else 
	// 	std::cout << this->_name << std::endl;
	// std::cout << this->_name << std::endl;
	// printMat(this->_model);
	// printMat(inverse(this->_model));
	// mat4 test = identityMat(1);

	// test = matMult(test, rotationMatX(this->_previous->_degree.x));
	// test = matMult(test, rotationMatY(this->_previous->_degree.y));
	// test = matMult(test, rotationMatZ(this->_previous->_degree.z));

	// this->_model = matMult(this->_model, test);
	// test = identityMat(1);

	// test = matMult(test, rotationMatX(this->_degree.x));
	// test = matMult(test, rotationMatY(this->_degree.y));
	// test = matMult(test, rotationMatZ(this->_degree.z));

	// mat4 test2 = upcastmat3(quatToMat(eulerToQuat(this->_degree.x * DEG2RAD, this->_degree.y * DEG2RAD, this->_degree.z * DEG2RAD)));

	// printMat(test);
	// printMat(test2);

	// this->_model = matMult(this->_model, test2);

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