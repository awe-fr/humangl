#include "./../includes/InputParser.hpp"

InputParser::InputParser(std::string asf_path, std::string amc_path) : _asf_path(asf_path), _amc_path(amc_path) {}

InputParser::~InputParser(void) {}



static void trim(std::string &line)
{
	size_t i = 0;

	while (i != line.size() && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r'))
		i++;

	line = line.substr(i);

	if (line[0] == '#')
		line = "";
}

static std::string parseKey(std::string &line)
{
	std::string key;
	size_t i = 0;

	if (line[0] == ':')
		i = 1;

	while (i != line.size() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
	{
		key += line[i];
		i++;
	}

	line = line.substr(i);

	return key;
}

static bool checkXYZ(std::string str)
{
	if (str == "XYZ" || str == "XZY"
		|| str == "YXZ" || str == "YZX"
		|| str == "ZXY" || str == "ZYX")
		return true;
	
	return false;
}



bool InputParser::parseAsfRootOrder(std::string &line, size_t &nb_line)
{
	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for root order." << std::endl;
		return false;
	}
	
	while (line.size() > 0)
	{
		std::string str;
		size_t i = 0;

		while (i != line.size() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
		{
			str += line[i];
			i++;
		}

		line = line.substr(i);

		if (std::find(this->_root_definition.order.begin(), this->_root_definition.order.end(), str) != this->_root_definition.order.end())
		{
			std::cerr << "ASF input file error: line " << nb_line << ": root order element '" << str << "' already defined." << std::endl;
			return false;
		}

		this->_root_definition.order.push_back(str);

		trim(line);
	}

	return true;
}

bool InputParser::parseAsfRootAxis(std::string &line, size_t &nb_line)
{
	std::string str;
	size_t i = 0;

	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for root order." << std::endl;
		return false;
	}

	while (i != line.size() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
	{
		str += line[i];
		i++;
	}

	line = line.substr(i);
	trim(line);

	if (line.size() != 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": multiple values for root axis." << std::endl;
		return false;
	}

	if (!checkXYZ(str))
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid root axis." << std::endl;
		return false;
	}

	this->_root_definition.axis = str;

	return true;
}

bool InputParser::parseAsfRootPosition(std::string &line, size_t &nb_line)
{
	size_t i = 0;

	trim(line);
	while (line.size() > 0 && i < 3)
	{
		std::string str;
		size_t j = 0;
		while (j != line.size() &&  line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j] != '\r')
		{
			str += line[j];
			j++;
		}

		switch (this->_root_definition.axis[i])
		{
			case 'X':
				this->_root_definition.position.x = (float)atof(str.c_str());
				break;

			case 'Y':
				this->_root_definition.position.y = (float)atof(str.c_str());
				break;
			
			case 'Z':
				this->_root_definition.position.z = (float)atof(str.c_str());

			default:
				break;
		}

		line = line.substr(j);
		trim(line);
		i++;
	}

	if (line.size() > 0 || i < 3)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid root position." << std::endl;
		return false;
	}

	return true;
}

bool InputParser::parseAsfRootOrientation(std::string &line, size_t &nb_line)
{
	size_t i = 0;

	trim(line);
	while (line.size() > 0 && i < 3)
	{
		std::string str;
		size_t j = 0;
		while (j != line.size() &&  line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j] != '\r')
		{
			str += line[j];
			j++;
		}

		switch (this->_root_definition.axis[i])
		{
			case 'X':
				this->_root_definition.orientation.x = (float)atof(str.c_str());
				break;

			case 'Y':
				this->_root_definition.orientation.y = (float)atof(str.c_str());
				break;
			
			case 'Z':
				this->_root_definition.orientation.z = (float)atof(str.c_str());

			default:
				break;
		}

		line = line.substr(j);
		trim(line);
		i++;
	}

	if (line.size() > 0 || i < 3)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid root orientation." << std::endl;
		return false;
	}

	return true;
}

bool InputParser::parseAsfRoot(size_t &nb_line)
{
	std::string line;
	bool is_order_parsed = false;
	bool is_axis_parsed = false;
	bool is_position_parsed = false;
	bool is_orientation_parsed = false;

	nb_line++;
	while (std::getline(this->_asf_file, line))
	{
		trim(line);
		if (line.size() == 0)
		{
			nb_line++;
			continue;
		}

		if (line[0] == ':')
		{
			std::cerr << "ASF input file error: line " << nb_line << ": missing fields for 'root'" << std::endl;
			return false;
		}

		std::string key = parseKey(line);
		if (key == ASF_KEY_ROOT_ORDER)
		{
			if (is_order_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for root order." << std::endl;
				return false;
			}
			if (!this->parseAsfRootOrder(line, nb_line))
				return false;

			is_order_parsed = true;
		}
		else if (key == ASF_KEY_ROOT_AXIS)
		{
			if (is_axis_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for root axis." << std::endl;
				return false;
			}
			if (!this->parseAsfRootAxis(line, nb_line))
				return false;

			is_axis_parsed = true;
		}
		else if (key == ASF_KEY_ROOT_POSITION)
		{
			if (is_position_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for root position." << std::endl;
				return false;
			}
			if (!is_axis_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": root axis should be defined before root position." << std::endl;
				return false;
			}
			if (!this->parseAsfRootPosition(line, nb_line))
				return false;

			is_position_parsed = true;
		}
		else if (key == ASF_KEY_ROOT_ORIENTATION)
		{
			if (is_orientation_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for root orientation." << std::endl;
				return false;
			}
			if (!is_axis_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": root axis should be defined before root orientation." << std::endl;
				return false;
			}
			if (!this->parseAsfRootOrientation(line, nb_line))
				return false;

			is_orientation_parsed = true;
		}

		nb_line++;

		if (is_order_parsed && is_axis_parsed && is_position_parsed && is_orientation_parsed)
			break;
	}

	if (!is_order_parsed || !is_axis_parsed || !is_position_parsed || !is_orientation_parsed)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing fields for 'root'" << std::endl;
		return false;
	}

	return true;
}



static std::string parseAsfBoneName(std::string &line)
{
	std::string str;
	size_t i = 0;

	trim(line);
	while (i != line.size() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
	{
		str += line[i];
		i++;
	}

	line = line.substr(i);

	return str;
}

static bool parseAsfBoneDirection(std::string &line, size_t &nb_line, vec3 &direction)
{
	size_t i = 0;

	trim(line);
	while (line.size() > 0 && i < 3)
	{
		std::string str;
		size_t j = 0;
		while (j != line.size() &&  line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j] != '\r')
		{
			str += line[j];
			j++;
		}

		switch (i)
		{
			case 0:
				direction.x = (float)atof(str.c_str());
				break;

			case 1:
				direction.y = (float)atof(str.c_str());
				break;
			
			case 2:
				direction.z = (float)atof(str.c_str());

			default:
				break;
		}

		line = line.substr(j);
		trim(line);
		i++;
	}

	if (line.size() > 0 || i < 3)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone direction." << std::endl;
		return false;
	}

	return true;
}

static bool parseAsfBoneLength(std::string &line, size_t &nb_line, float &length)
{
	std::string str;
	size_t i = 0;

	trim(line);
	while (i != line.size() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
	{
		str += line[i];
		i++;
	}

	trim(line);
	if (str.size() == 0 || line.size() != 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone length." << std::endl;
		return false;
	}

	length = (float)atof(str.c_str());

	return true;
}

static bool parseAsfBoneAxis(std::string &line, size_t &nb_line, vec3 &axis)
{
	size_t i = 0;
	float tab[3];

	trim(line);
	while (line.size() > 0 && i < 3)
	{
		std::string str;
		size_t j = 0;
		while (j != line.size() &&  line[j] != ' ' && line[j] != '\t' && line[j] != '\n' && line[j] != '\r')
		{
			str += line[j];
			j++;
		}

		tab[i] = (float)atof(str.c_str());

		line = line.substr(j);
		trim(line);
		i++;
	}

	if (i < 3)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone axis" << std::endl;
		return false;
	}

	if (line.size() == 0)
	{
		axis.x = tab[0];
		axis.y = tab[1];
		axis.z = tab[2];
	}
	else
	{
		std::string str;
		i = 0;
		while (i != line.size() &&  line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
		{
			str += line[i];
			i++;
		}

		if (str.size() != 3)
		{
			std::cerr << "ASF input file error: line " << nb_line << ": invalid bone axis" << std::endl;
			return false;
		}

		if (!checkXYZ(str))
		{
			std::cerr << "ASF input file error: line " << nb_line << ": invalid bone axis" << std::endl;
			return false;
		}

		for (size_t j = 0; j < 3; j++)
		{
			switch (str[j])
			{
				case 'X':
					axis.x = tab[j];
					break;

				case 'Y':
					axis.y = tab[j];
					break;

				case 'Z':
					axis.z = tab[j];
					break;
				
				default:
					break;
			}
		}
	}

	return true;
}

static bool parseAsfBoneDof(std::string &line, size_t &nb_line, std::map<std::string, Limit> &dof)
{


	return true;
}

bool InputParser::parseAsfBone(size_t &nb_line)
{
	BoneDefinition bone;
	bool is_name_parsed = false;
	bool is_direction_parsed = false;
	bool is_length_parsed = false;
	bool is_axis_parsed = false;
	bool is_dof_parsed = false;
	bool is_limits_parsed = false;

	std::string line;

	while (std::getline(this->_asf_file, line))
	{
		trim(line);
		if (line.size() == 0)
		{
			nb_line++;
			continue;
		}

		std::string key = parseKey(line);
		if (key == ASF_KEY_BONEDATA_NAME)
		{
			if (is_name_parsed)
			{
				std::cerr << "ASF input file error: line" << nb_line << ": multiple definitions for bone name." << std::endl;
				return false;
			}

			bone.name = parseAsfBoneName(line);
			trim(line);
			if (bone.name.size() == 0 || line.size() != 0)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": bone invalid name." << std::endl;
				return false;
			}
			if (this->_bonedata.find(bone.name) != this->_bonedata.end())
			{
				std::cerr << "ASF input file error: line " << nb_line << ": bone '" << bone.name << "' already exists." << std::endl;
				return false;
			}

			is_name_parsed = true;
		}
		else if (key == ASF_KEY_BONEDATA_DIRECTION)
		{
			if (is_direction_parsed)
			{
				std::cerr << "ASF input file error: line" << nb_line << ": multiple definitions for bone direction." << std::endl;
				return false;
			}
			if (!parseAsfBoneDirection(line, nb_line, bone.direction))
				return false;

			is_direction_parsed = true;
		}
		else if (key == ASF_KEY_BONEDATA_LENGTH)
		{
			if (is_length_parsed)
			{
				std::cerr << "ASF input file error: line" << nb_line << ": multiple definitions for bone length." << std::endl;
				return false;
			}
			if (!parseAsfBoneLength(line, nb_line, bone.length))
				return false;

			is_length_parsed = true;
		}
		else if (key == ASF_KEY_BONEDATA_AXIS)
		{
			if (is_axis_parsed)
			{
				std::cerr << "ASF input file error: line" << nb_line << ": multiple definitions for bone axis." << std::endl;
				return false;
			}
			if (!parseAsfBoneAxis(line, nb_line, bone.axis))
				return false;

			is_axis_parsed = true;
		}
		else if (key == ASF_KEY_BONEDATA_DOF)
		{
			if (is_dof_parsed)
			{
				std::cerr << "ASF input file error: line" << nb_line << ": multiple definitions for bone dof." << std::endl;
				return false;
			}
			if (!parseAsfBoneDof(line, nb_line, bone.dof))
				return false;

			is_dof_parsed = true;
		}
	}
}

bool InputParser::parseAsfBonedata(size_t &nb_line)
{
	std::string line;

	nb_line++;
	while (std::getline(this->_asf_file, line))
	{
		trim(line);
		if (line.size() == 0)
		{
			nb_line++;
			continue;
		}

		if (line[0] == ':')
			break;

		std::string key = parseKey(line);
		if (key == ASF_KEY_BEGIN)
		{
			if (!this->parseAsfBone(nb_line))
				return false;
		}

		nb_line++;
	}

	return true;
}



bool InputParser::parseAsf(void)
{
	this->_asf_file.open(this->_asf_path);

	if (!this->_asf_file)
	{
		std::cerr << "ASF input file error: file '" << this->_asf_path << "' does not exist." << std::endl;
		return false;
	}

	bool is_root_definition_parsed = false;
	bool is_bonedata_parsed = false;
	bool is_hierarchy_parsed = false;

	std::string line;
	size_t nb_line = 1;

	while (std::getline(this->_asf_file, line))
	{
		trim(line);
		if (line.size() == 0)
		{
			nb_line++;
			continue;
		}

		if (line[0] == ':')
		{
			std::string key = parseKey(line);
			if (key == ASF_KEY_ROOT)
			{
				if (is_root_definition_parsed)
				{
					std::cerr << "ASF input file error: line " << nb_line << ": multiple defnition for 'root'." << std::endl;
					this->_asf_file.close();
					return false;
				}
				if (!this->parseAsfRoot(nb_line))
				{
					this->_asf_file.close();
					return false;
				}

				is_root_definition_parsed = true;
			}
			else if (key == ASF_KEY_BONEDATA)
			{
				if (is_bonedata_parsed)
				{
					std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for 'bonedata'." << std::endl;
					this->_asf_file.close();
					return false;
				}
				if (!this->parseAsfBonedata(nb_line))
				{
					this->_asf_file.close();
					return false;
				}

				is_bonedata_parsed = true;
			}
		}

		nb_line++;
	}

	return true;
}

bool InputParser::parseAmc(void)
{


	return true;
}