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
		float length = sqrtf(powf(offset.x, 2) + powf(offset.y, 2) + powf(offset.z, 2));
		parent->SetLength(length);

		// Normalize offset
		offset.x = offset.x / length;
		offset.y = offset.y / length;
		offset.z = offset.z / length;

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
			break;
		}
		else if (!is_joint)
			throw Exception(MissingKeyException("End Site"));

		is_complete = true;
		break;
	}

	if (!is_complete)
		throw Exception(IncompleteFile());

	ParseBracket(file, '}');
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
			while (j < line.length() && line[j] != ' ' && line[j] == '\t' && line[j] == '\n' && line[j] == '\r')
				j++;

			frame.push_back(ParseFloat(line.substr(0, j)));

			line = line.substr(j);
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

	try
	{
		file.open(this->_file_path);
	}
	catch(std::exception& e)
	{
		throw Exception(CannotOpenFile());
	}
	
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

	file.close();
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