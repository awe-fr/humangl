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

	if (name == "head") {
		this->_vertex[0] = length / 2;		this->_vertex[1] = length;  this->_vertex[2] = length / 2;
		this->_vertex[3] = -length / 2;		this->_vertex[4] = length;  this->_vertex[5] = -length / 2;
		this->_vertex[6] = -length / 2;		this->_vertex[7] = length;  this->_vertex[8] = length / 2;
		this->_vertex[9] = length / 2;		this->_vertex[10] = 0;      this->_vertex[11] = -length / 2;
		this->_vertex[12] = -length / 2;	this->_vertex[13] = 0;      this->_vertex[14] = -length / 2;
		this->_vertex[15] = length / 2;   	this->_vertex[16] = length; this->_vertex[17] = -length / 2;
		this->_vertex[18] = length / 2;	this->_vertex[19] = 0;      this->_vertex[20] = length / 2;
		this->_vertex[21] = -length / 2;	this->_vertex[22] = 0;      this->_vertex[23] = length / 2;
	}

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
		if (this->_name == "lfemur") {
			rot = eulerToQuat(-39.022781 * DEG2RAD, -17.852474 * DEG2RAD, -14.329288 * DEG2RAD);
		}
		else if (this->_name == "ltibia") {
			rot = eulerToQuat((-39.022781 + 37.413830) * DEG2RAD, (-17.852474 + 12.469515) * DEG2RAD, (-14.329288 + 4.237188) * DEG2RAD);
		}
		else if (this->_name == "lfoot") {
			rot = eulerToQuat(((-39.022781 + 37.413830) + -13.110087) * DEG2RAD, ((-17.852474 + 12.469515) + 5.382885) * DEG2RAD, ((-46.022781 + 37.413830) + 0.259709) * DEG2RAD);
		}
		else if (this->_name == "ltoes") {
			rot = eulerToQuat((((-39.022781 + 37.413830) + -13.110087) + -9.526491) * DEG2RAD, (((-17.852474 + 12.469515) + 5.382885) + -3.447239) * DEG2RAD, (((-46.022781 + 37.413830) + 0.259709) + 0.287332) * DEG2RAD);
		}		
		else if (this->_name == "rfemur") {
			rot = eulerToQuat(14.253702 * DEG2RAD, -8.037724 * DEG2RAD, 24.373158 * DEG2RAD);
		}
		else if (this->_name == "rtibia") {
			rot = eulerToQuat((14.253702 + 9.132729) * DEG2RAD, (-8.037724 + -3.306309) * DEG2RAD, (24.373158 + -0.264139) * DEG2RAD);
		}
		else if (this->_name == "rfoot") {
			rot = eulerToQuat(((14.253702 + 9.132729) + -20.482639) * DEG2RAD, ((-8.037724 + -3.306309) + 1.373469) * DEG2RAD, ((24.373158 + -0.264139) + -1.220988) * DEG2RAD);
		}
		else if (this->_name == "rtoes") {
			rot = eulerToQuat((((14.253702 + 9.132729) + -20.482639) + 1.736804) * DEG2RAD, (((-8.037724 + -3.306309) + 1.373469) + -0.632022) * DEG2RAD, (((24.373158 + -0.264139) + -1.220988) + -0.009581) * DEG2RAD);
		}		
		else if (this->_name == "lowerback") {
			rot = eulerToQuat((12.576200) * DEG2RAD, (-5.611799) * DEG2RAD, (2.372790) * DEG2RAD);
		}
		else if (this->_name == "upperback") {
			rot = eulerToQuat((12.576200 + 3.178510) * DEG2RAD, (0.611799 + 1.774400) * DEG2RAD, (7.372790 + 0.829941) * DEG2RAD);
		}
		else if (this->_name == "thorax") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880) * DEG2RAD, (0.611799 + 1.774400 + 0.834225) * DEG2RAD, (7.372790 + 0.829941 + -3.554390) * DEG2RAD);
		}
		else if (this->_name == "lowerneck") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + -12.191300) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 1.139920) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.355122) * DEG2RAD);
		}
		else if (this->_name == "upperneck") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + -12.191300 + -6.788760) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 1.139920 + 1.361360) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.355122 + -4.967230) * DEG2RAD);
		}
		else if (this->_name == "head") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + -12.191300 + -6.788760 + -0.054032) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 1.139920 + 1.361360 + 0.627240) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.355122 + -4.967230 + -1.565310) * DEG2RAD);
		}
		else if (this->_name == "lclavicle") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35) * DEG2RAD);
		}
		else if (this->_name == "lhumerus") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + -16.422426) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 12.359011) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + -91.953796) * DEG2RAD);
		}
		else if (this->_name == "lradius") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + -16.422426 + -0.804702) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 12.359011 + -12.486639) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + -91.953796 + 7.345614) * DEG2RAD);
		}
		else if (this->_name == "lwrist") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + -16.422426 + -0.804702 + -12.363196) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 12.359011 + -12.486639 + 0) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + -91.953796 + 7.345614 + 0) * DEG2RAD);
		}
		else if (this->_name == "lhand") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + -16.422426 + -0.804702 + -12.363196 + 8.562183) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 12.359011 + -12.486639 + 0 + 17.307680) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + -91.953796 + 7.345614 + 0 + 26.843103) * DEG2RAD);
		}
		else if (this->_name == "lfingers") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + -16.422426 + -0.804702 + -12.363196 + 8.562183 + 0) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 12.359011 + -12.486639 + 0 + 17.307680 + 0) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + -91.953796 + 7.345614 + 0 + 26.843103 + -7.125019) * DEG2RAD);
		}
		else if (this->_name == "lthumb") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + -16.422426 + -0.804702 + -12.363196 + 8.562183 + -0.112050) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 12.359011 + -12.486639 + 0 + 17.307680 + 49.228218) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + -91.953796 + 7.345614 + 0 + 26.843103 + -1.074497) * DEG2RAD);
		}
		else if (this->_name == "rclavicle") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35) * DEG2RAD);
		}
		else if (this->_name == "rhumerus") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + 46.967899) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 18.733820) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + 78.647438) * DEG2RAD);
		}
		else if (this->_name == "rradius") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + 46.967899 + -17.181623) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 18.733820 + 47.027115) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + 78.647438 + -38.295696) * DEG2RAD);
		}
		else if (this->_name == "rwrist") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + 46.967899 + -17.181623 + -21.534393) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 18.733820 + 47.027115 + 0) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + 78.647438 + -38.295696 + 0) * DEG2RAD);
		}
		else if (this->_name == "lhand") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + 46.967899 + -17.181623 + -21.534393 + -11.491422) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 18.733820 + 47.027115 + 0 + 18.305342) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + 78.647438 + -38.295696 + 0 + -32.913929) * DEG2RAD);
		}
		else if (this->_name == "lfingers") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + 46.967899 + -17.181623 + -21.534393 + -11.491422 + 0) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 18.733820 + 47.027115 + 0 + 18.305342 + 0) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + 78.647438 + -38.295696 + 0 + -32.913929 + 7.125019) * DEG2RAD);
		}
		else if (this->_name == "rthumb") {
			rot = eulerToQuat((12.576200 + 3.178510 + -3.497880 + 0 + 46.967899 + -17.181623 + -21.534393 + -2.611591) * DEG2RAD, (0.611799 + 1.774400 + 0.834225 + 0 + 18.733820 + 47.027115 + 0 + -8.497252) * DEG2RAD, (7.372790 + 0.829941 + -3.554390 + 0.35 + 78.647438 + -38.295696 + 0 + -3.099707) * DEG2RAD);
		}
		else		
			rot = eulerToQuat(this->_degree.x * DEG2RAD, this->_degree.y * DEG2RAD, this->_degree.z * DEG2RAD);


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
			if (this->_name != "head") {

				this->_length = ratio->value * this->_base_length;
				
				this->_vertex[1] = this->_length;
				this->_vertex[4] = this->_length;
				this->_vertex[7] = this->_length;
				this->_vertex[16] = this->_length;
			}
			break;

		case Width:
			if (this->_name != "head") {
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
			}
			break;
		
		case Head_size:
			if (this->_name == "head") {
				this->_length = ratio->value * this->_base_length;
				
				this->_vertex[1] = this->_length;
				this->_vertex[4] = this->_length;
				this->_vertex[7] = this->_length;
				this->_vertex[16] = this->_length;
				
				this->_vertex[9] = this->_length / 2;
				this->_vertex[12] = -this->_length / 2;
				this->_vertex[18] = this->_length / 2;
				this->_vertex[21] = -this->_length / 2;
				this->_vertex[11] = -this->_length / 2;
				this->_vertex[14] = -this->_length / 2;
				this->_vertex[20] = this->_length / 2;
				this->_vertex[23] = this->_length / 2;

				this->_vertex[0] = this->_length / 2;
				this->_vertex[3] = -this->_length / 2;
				this->_vertex[6] = -this->_length / 2;
				this->_vertex[15] = this->_length / 2;
				this->_vertex[2] = this->_length / 2;
				this->_vertex[5] = -this->_length / 2;
				this->_vertex[8] = this->_length / 2;
				this->_vertex[17] = -this->_length / 2;
			}
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