# include "../includes/BVHAnimation.hpp"
# include <iostream>
# include <fstream>
# include <sstream>
# include <cctype>
# include <cmath>

static void trim(std::string &line)
{
	int i = 0;

	while (i != line.length() && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r'))
		i++;

	line = line.substr(i);

	if (line.length() == 0)
		return;

	i = line.length() - 1;
	while (i > -1 && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r'))
		i--;

	line = line.substr(0, i + 1);

	if (line[0] == '#')
		line = "";
}

static bool startsWith(const std::string str, const std::string prefix)
{
	if (str.substr(0, prefix.length()) == prefix)
		return true;

	return false;
}

static std::string size_tToString(size_t s)
{
	std::stringstream ss;

	ss << s;
	return ss.str();
}





const size_t BVHAnimation::GetTotalChannelsNumber(void)
{
	size_t nb = 0;

	for (size_t i = 0; i < this->_members.size(); i++)
		nb += this->_members[i]->GetChannelsNumber();

	return nb;
}





float BVHAnimation::ParseFloat(std::string str)
{
	bool point = false;

	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isdigit(str[i]))
		{
			char c = str[i];

			if ((c == '-' || c == '+') && i == 0)
				continue;	
			else if (c == '.' && !point && i > 0 && i < str.length() - 1)
				point = true;
			else
				throw Exception(InvalidFloat());
		}
	}
	return float(atof(str.c_str()));
}

size_t BVHAnimation::ParseSizeT(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isdigit(str[i]))
			throw Exception(InvalidSizeT());
	}

	std::stringstream ss(str);
	size_t ret;

	ss >> ret;
	return ret;
}

void BVHAnimation::ParseBracket(std::ifstream &file, char bracket)
{
	std::string line;
	bool is_complete = false;

	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;

		if (line.length() != 1 && line[0] != bracket)
		{
			switch (bracket)
			{
				case '{':
					throw Exception(MissingLeftBracket());

				case '}':
					throw Exception(MissingRightBracket());
				
				default:
					break;
			}
		}
		
		is_complete = true;
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());
}

vec3 BVHAnimation::ParseOffset(std::string line)
{
	vec3 offset;
	size_t i = 0;

	line = line.substr(6);
	while (i < 3)
	{
		trim(line);
		if (line.length() == 0)
			break;

		size_t j = 0;
		while (j < line.length() && line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j] != '\r')
			j++;

		switch (i)
		{
			case 0:
				offset.x = ParseFloat(line.substr(0, j));
				break;

			case 1:
				offset.y = ParseFloat(line.substr(0, j));
				break;

			case 2:
				offset.z = ParseFloat(line.substr(0, j));
				break;
			
			default:
				break;
		}

		line = line.substr(j);
		i++;
	}
	trim(line);
	if (i < 3 || line.length() > 0)
		throw Exception(InvalidOffset());

	return offset;
}

BVHAnimation::Member::Channel BVHAnimation::GetChannel(std::string str)
{
	if (str == "Zrotation")
		return BVHAnimation::Member::Channel::ZROT;
	if (str == "Yrotation")
		return BVHAnimation::Member::Channel::YROT;
	if (str == "Xrotation")
		return BVHAnimation::Member::Channel::XROT;
	if (str == "Zposition")
		return BVHAnimation::Member::Channel::ZPOS;
	if (str == "Yposition")
		return BVHAnimation::Member::Channel::YPOS;
	if (str == "Xposition")
		return BVHAnimation::Member::Channel::XPOS;
	
	throw Exception(InvalidChannelElement());
}

std::vector<BVHAnimation::Member::Channel> BVHAnimation::ParseChannels(std::string line)
{
	std::vector<BVHAnimation::Member::Channel> channels;

	line = line.substr(8);
	trim(line);

	size_t i = 0;
	while (i != line.length() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
		i++;

	if (i > 1 || !std::isdigit(line[0]))
		throw Exception(InvalidChannelNumber());

	size_t channel_number = (size_t)atoi(line.substr(0, 1).c_str());
	if (channel_number > 6)
		throw Exception(InvalidChannelNumber());

	line = line.substr(1);

	i = 0;
	while (i < channel_number)
	{
		trim(line);
		if (line.length() == 0)
			break;

		size_t j = 0;
		while (j < line.length() && line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j] != '\r')
			j++;

		channels.push_back(GetChannel(line.substr(0, j)));

		line = line.substr(j);
		i++;
	}
	
	trim(line);
	if (i < channel_number)
		throw Exception(MissingChannelElement(channel_number, i));
	if (line.length() > 0)
		throw Exception(MissingChannelElement(channel_number, i + 1));
	
	return channels;
}





void BVHAnimation::ParseMember(std::ifstream &file, BVHAnimation::Member &member)
{
	std::string line;
	bool is_complete = false;

	ParseBracket(file, '{');

	// OFFSET
	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;

		if (!startsWith(line, "OFFSET"))
			throw Exception(MissingKeyException("OFFSET"));

		is_complete = true;
		vec3 offset = ParseOffset(line);

		Member *parent = member.GetParent();
		if (parent == nullptr)
			break;

		// Set length
		float length = sqrtf((offset.x * offset.x) + (offset.y * offset.y) + (offset.z * offset.z));
		parent->SetLength(length);

		// Normalize offset	
		if (length != 0) {
			offset.x = offset.x / length;
			offset.y = offset.y / length;
			offset.z = offset.z / length;
		} else {
			offset = {0, 0, 0};
		}
		parent->SetOffset(offset);
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());

	is_complete = false;

	// CHANNELS
	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;

		if (!startsWith(line, "CHANNELS"))
			throw Exception(MissingKeyException("CHANNELS"));

		is_complete = true;
		member.SetChannels(ParseChannels(line));
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());

	is_complete = false;

	// JOINT or End Site
	bool is_joint = false;
	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;

		if (startsWith(line, "JOINT"))
		{
			line = line.substr(5);
			trim(line);

			Member* child = new Member(line, &member);
			member.AddChild(child);
			this->_members.push_back(child);

			ParseMember(file, *child);

			is_joint = true;
			continue;
		}
		else if (line == "End Site")
		{
			ParseBracket(file, '{');

			is_complete = false;

			// OFFSET
			while (getline(file, line))
			{
				this->_nb_line++;
				trim(line);
				if (line.length() == 0)
					continue;

				if (!startsWith(line, "OFFSET"))
					throw Exception(MissingKeyException("OFFSET"));

				vec3 offset = ParseOffset(line);
				
				// Set length
				float length = sqrtf(powf(offset.x, 2) + powf(offset.y, 2) + powf(offset.z, 2));
				member.SetLength(length);
				
				// Normalize offset
				offset.x = offset.x / length;
				offset.y = offset.y / length;
				offset.z = offset.z / length;
				
				member.SetOffset(offset);
				is_complete = true;
				break;
			}

			if (!is_complete)
				throw Exception(IncompleteFile());

			ParseBracket(file, '}');
			getline(file, line);
			this->_nb_line++;
			trim(line);
			break;
		}
		else if (!is_joint)
			throw Exception(MissingKeyException("End Site"));
		
		is_complete = true;
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());

	if (line != "}")
		throw Exception(MissingRightBracket());
}





void BVHAnimation::ParseAnimation(std::ifstream &file)
{
	std::string line;
	size_t nb_channels = GetTotalChannelsNumber();

	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;

		std::vector<float> frame;
		size_t i = 0;
		while (i < nb_channels)
		{
			trim(line);
			if (line.length() == 0)
				break;

			size_t j = 0;
			while (j < line.length() && line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j] != '\r')
				j++;

			frame.push_back(ParseFloat(line.substr(0, j)));

			line = line.substr(j);
			i++;
		}

		trim(line);
		if (i < nb_channels)
			throw Exception(MissingChannelElement(nb_channels, i));
		if (line.length() > 0)
			throw Exception(MissingChannelElement(nb_channels, i + 1));
	
		this->_animation.push_back(frame);
	}

	if (this->_animation.size() != this->_nb_frames)
		throw Exception(MissingNbFrames(this->_nb_frames, this->_animation.size()));
}





void BVHAnimation::Parse(void)
{
	std::ifstream file;

	file.open(this->_file_path);
	if (!file.is_open())
		throw Exception(CannotOpenFile());
	
	this->_nb_line = 0;
	bool is_complete = false;
	
	std::string line;

	// HIERARCHY
	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;
		
		if (line != "HIERARCHY")
			throw Exception(MissingKeyException("HIERARCHY"));

		is_complete = true;
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());

	is_complete = false;

	// ROOT
	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;
		
		if (!startsWith(line, "ROOT"))
			throw Exception(MissingKeyException("ROOT"));

		is_complete = true;
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());

	this->_members.push_back(&this->_root);
	ParseMember(file, this->_root);

	is_complete = false;

	// MOTION
	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;
		
		if (line != "MOTION")
			throw Exception(MissingKeyException("MOTION"));

		is_complete = true;
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());

	is_complete = false;

	// Frames
	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;
		
		if (!startsWith(line, "Frames:"))
			throw Exception(MissingKeyException("Frames"));

		line = line.substr(7);
		trim(line);

		this->_nb_frames = ParseSizeT(line);

		is_complete = true;
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());

	is_complete = false;

	// Frame Time
	while (getline(file, line))
	{
		this->_nb_line++;
		trim(line);
		if (line.length() == 0)
			continue;
		
		if (!startsWith(line, "Frame Time:"))
			throw Exception(MissingKeyException("Frame Time"));

		line = line.substr(11);
		trim(line);

		this->_frame_time = ParseFloat(line);

		is_complete = true;
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());

	// Animation
	ParseAnimation(file);

	InitMembersGraphic();

	file.close();
	this->_is_parsed = true;
}





// Exception messages
const std::string BVHAnimation::ExceptionHeader(void)
{
	std::string message;

	message += "BVHAnimation exception line " + size_tToString(this->_nb_line) + " for:\n";
	message += "  Name: " + this->_name + "\n";
	message += "  Path: " + this->_file_path + "\n";

	return message;
}

const std::string BVHAnimation::MissingKeyException(const std::string key)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Missing key '" + key + "'\n";

	return message;
}

const std::string BVHAnimation::CannotOpenFile(void)
{
	std::string message = ExceptionHeader();
	message += "Impossible to open file.\n";

	return message;
}

const std::string BVHAnimation::MissingLeftBracket(void)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Missing opening bracket.\n";

	return message;
}

const std::string BVHAnimation::MissingRightBracket(void)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Missing closing bracket.\n";

	return message;
}

const std::string BVHAnimation::IncompleteFile(void)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Missing informations.\n";

	return message;
}

const std::string BVHAnimation::InvalidFloat(void)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Invalid float.\n";

	return message;
}

const std::string BVHAnimation::InvalidSizeT(void)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Invalid unsigned int.\n";

	return message;
}

const std::string BVHAnimation::InvalidOffset(void)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Invalid Offset: expected 3 float values.\n";

	return message;
}

const std::string BVHAnimation::InvalidChannelNumber(void)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Invalid Channels: Invalid channel number.\n";

	return message;
}

const std::string BVHAnimation::InvalidChannelElement(void)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Invalid Channels: Invalid channel element.\n";

	return message;
}

const std::string BVHAnimation::DuplicateChannelElement(void)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Invalid Channels: Duplicate channel element.\n";

	return message;
}

const std::string BVHAnimation::MissingChannelElement(size_t expected, size_t got)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Invalid Channels: Expected " + size_tToString(expected) + ", got " + size_tToString(got) + ".\n";

	return message;
}

const std::string BVHAnimation::MissingFrameElement(size_t expected, size_t got)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Invalid Frame: Expected " + size_tToString(expected) + ", got " + size_tToString(got) + ".\n";

	return message;
}

const std::string BVHAnimation::MissingNbFrames(size_t expected, size_t got)
{
	std::string message = ExceptionHeader();
	message += "Invalid file format. Invalid Animation: Expected " + size_tToString(expected) + ", got " + size_tToString(got) + ".\n";

	return message;
}

const std::string BVHAnimation::NotParsed(void)
{
	std::string message;
	
	message += "BVHAnimation exception for:\n";
	message += "  Name: " + this->_name + "\n";
	message += "  Path: " + this->_file_path + "\n";
	message += "Cannot run. The file is not parsed.";
	
	return message;
}

void BVHAnimation::Run(void)
{
	if (!this->_is_parsed)
		throw Exception(NotParsed());

	static double start_time = glfwGetTime();
	double time_elapsed = glfwGetTime() - start_time;
	size_t frame_number = (size_t)(time_elapsed / this->_frame_time);
	
	if (frame_number >= this->_nb_frames)
	{
		start_time = glfwGetTime();
		frame_number = 0;
	}

	std::vector<float> frame = this->_animation[frame_number];

	size_t frame_elt_id = 0;

	for (size_t j = 0; j < this->_members.size(); j++)
	{
		Member *member = this->_members[j];
		std::vector<Member::Channel> channels = member->GetChannels();

		for (size_t k = 0; k < channels.size(); k++)
		{
			Root* rootMember = dynamic_cast<Root*>(member);

			float frame_elt = frame[frame_elt_id];

			switch (channels[k])
			{
				case Member::Channel::ZROT:
					member->setRotZ(frame_elt);
					break;

				case Member::Channel::YROT:
					member->setRotY(frame_elt);
					break;

				case Member::Channel::XROT:
					member->setRotX(frame_elt);
					break;

				case Member::Channel::XPOS:
					if (rootMember != nullptr) {
						rootMember->setTransX(frame_elt);
					}
					break;

				case Member::Channel::YPOS:
					if (rootMember != nullptr) {
						rootMember->setTransY(frame_elt);
					}
					break;

				case Member::Channel::ZPOS:
					if (rootMember != nullptr) {
						rootMember->setTransZ(frame_elt);
					}
					break;
				
				default:
					break;
			}

			frame_elt_id++;
		}
	}
}

void BVHAnimation::InitMembersGraphic(void) {
	for (int i = 0; i < this->_members.size(); i++) {
		this->_members[i]->initGraphics();
		
		Root* rootMember = dynamic_cast<Root*>(this->_members[i]);
        if (rootMember != nullptr) {
            rootMember->initTrans();
        }
	}
}

void BVHAnimation::Member::initGraphics(void) {
	this->_model = identityMat(1);

	this->_vertex = new GLfloat[sizeof(GLfloat) * 24];
	this->_vertex[0] = MEMBER_BASE_WIDTH_END;		this->_vertex[1] = this->_length;  this->_vertex[2] = MEMBER_BASE_WIDTH_END;
	this->_vertex[3] = -MEMBER_BASE_WIDTH_END;		this->_vertex[4] = this->_length;  this->_vertex[5] = -MEMBER_BASE_WIDTH_END;
	this->_vertex[6] = -MEMBER_BASE_WIDTH_END;		this->_vertex[7] = this->_length;  this->_vertex[8] = MEMBER_BASE_WIDTH_END;
	this->_vertex[9] = MEMBER_BASE_WIDTH_START;		this->_vertex[10] = 0;      this->_vertex[11] = -MEMBER_BASE_WIDTH_START;
	this->_vertex[12] = -MEMBER_BASE_WIDTH_START;	this->_vertex[13] = 0;      this->_vertex[14] = -MEMBER_BASE_WIDTH_START;
	this->_vertex[15] = MEMBER_BASE_WIDTH_END;   	this->_vertex[16] = this->_length; this->_vertex[17] = -MEMBER_BASE_WIDTH_END;
	this->_vertex[18] = MEMBER_BASE_WIDTH_START;	this->_vertex[19] = 0;      this->_vertex[20] = MEMBER_BASE_WIDTH_START;
	this->_vertex[21] = -MEMBER_BASE_WIDTH_START;	this->_vertex[22] = 0;      this->_vertex[23] = MEMBER_BASE_WIDTH_START;

	if (this->_name == "head") {
		this->_vertex[0] = this->_length / 2;		this->_vertex[1] = this->_length;  	this->_vertex[2] = this->_length / 2;
		this->_vertex[3] = -this->_length / 2;		this->_vertex[4] = this->_length;  	this->_vertex[5] = -this->_length / 2;
		this->_vertex[6] = -this->_length / 2;		this->_vertex[7] = this->_length;  	this->_vertex[8] = this->_length / 2;
		this->_vertex[9] = this->_length / 2;		this->_vertex[10] = 0;      		this->_vertex[11] = -this->_length / 2;
		this->_vertex[12] = -this->_length / 2;		this->_vertex[13] = 0;      		this->_vertex[14] = -this->_length / 2;
		this->_vertex[15] = this->_length / 2;   	this->_vertex[16] = this->_length; 	this->_vertex[17] = -this->_length / 2;
		this->_vertex[18] = this->_length / 2;		this->_vertex[19] = 0;      		this->_vertex[20] = this->_length / 2;
		this->_vertex[21] = -this->_length / 2;		this->_vertex[22] = 0;      		this->_vertex[23] = this->_length / 2;
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

	this->_degree = {0,0,0};
}

void BVHAnimation::Member::computeTravel2() {
		quat q = quatMat2({this->_offset.x,this->_offset.y,this->_offset.z}, {0,1,0});

		mat4 rotToChild = upcastmat3(quatToMat(q));
	
		this->_model = matMult(this->_model, rotToChild);
}

void BVHAnimation::Member::computeTravel() {
	this->_model = identityMat(1);

	if (this->_parent != nullptr) {

		this->_model = this->_parent->_model;

		this->_model = (matMult(this->_model, translationMat(this->_parent->_length * this->_parent->_offset.x, this->_parent->_length * this->_parent->_offset.y, this->_parent->_length * this->_parent->_offset.z)));

		quat rot = eulerToQuat((this->_degree.x) * DEG2RAD, (this->_degree.y) * DEG2RAD, (this->_degree.z) * DEG2RAD);

		this->_model = matMult(this->_model, upcastmat3(quatToMat(rot)));

	} else if (this->_parent == nullptr) {
		Root* rootMember = dynamic_cast<Root*>(this);
		rootMember->computeTravelRoot();
	}
}

void	BVHAnimation::Root::computeTravelRoot() {
	this->_model = matMult(this->_model, translationMat(this->_transDegree.x, this->_transDegree.y, this->_transDegree.z));
	quat rot = eulerToQuat((this->_degree.x) * DEG2RAD, (this->_degree.y) * DEG2RAD, (this->_degree.z) * DEG2RAD);
	this->_model = matMult(this->_model, upcastmat3(quatToMat(rot)));
}

GLuint BVHAnimation::Member::getVAO() {
	return (this->_vao);
}

GLuint BVHAnimation::Member::getVBO() {
	return (this->_vbo);
}

GLuint BVHAnimation::Member::getIBO() {
	return (this->_ibo);
}

mat4 BVHAnimation::Member::getModel() {
	return (this->_model);
}

void BVHAnimation::Member::update(void *param) {
	Param *ratio = (Param *)param;

	switch (ratio->type)
	{
		case Length:
			if (this->_name != "head") {

				this->_length = ratio->value * this->_length_ratio;
				
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
				this->_length = ratio->value * this->_length_ratio;
				
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

BVHAnimation::Member::~Member() {
	glDeleteBuffers(1, &this->_vbo);
	glDeleteBuffers(1, &this->_ibo);
	glDeleteVertexArrays(1, &this->_vao);

	delete[] this->_vertex;
	delete[] this->_index;
}