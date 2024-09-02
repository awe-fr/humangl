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



bool InputParser::parseASFRootOrder(std::string &line, size_t &nb_line)
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

bool InputParser::parseASFRootAxis(std::string &line, size_t &nb_line)
{
	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for root axis." << std::endl;
		return false;
	}

	std::string str;
	size_t i = 0;
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

bool InputParser::parseASFRootPosition(std::string &line, size_t &nb_line)
{
	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for root position." << std::endl;
		return false;
	}

	size_t i = 0;
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

bool InputParser::parseASFRootOrientation(std::string &line, size_t &nb_line)
{
	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for root orientation." << std::endl;
		return false;
	}

	size_t i = 0;
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

bool InputParser::parseASFRoot(size_t &nb_line)
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
			if (!this->parseASFRootOrder(line, nb_line))
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
			if (!this->parseASFRootAxis(line, nb_line))
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
			if (!this->parseASFRootPosition(line, nb_line))
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
			if (!this->parseASFRootOrientation(line, nb_line))
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



bool InputParser::parseASFBoneName(std::string &line, size_t &nb_line, std::string &name)
{
	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for bone name." << std::endl;
		return false;
	}

	std::string str;
	size_t i = 0;

	while (i != line.size() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
	{
		str += line[i];
		i++;
	}

	line = line.substr(i);
	trim(line);

	if (line.size() != 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone name." << std::endl;
		return false;
	}

	if (this->_bonedata.find(str) != this->_bonedata.end())
	{
		std::cerr << "ASF input file error: line " << nb_line << ": bone name '" << str << "' already used." << std::endl;
		return false;
	}

	name = str;

	return true;
}

static bool parseASFBoneDirection(std::string &line, size_t &nb_line, vec3 &direction)
{
	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for bone direction." << std::endl;
		return false;
	}

	size_t i = 0;
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

static bool parseASFBoneLength(std::string &line, size_t &nb_line, float &length)
{
	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for bone length." << std::endl;
		return false;
	}

	std::string str;
	size_t i = 0;

	while (i != line.size() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
	{
		str += line[i];
		i++;
	}

	line = line.substr(i);

	trim(line);
	if (str.size() == 0 || line.size() != 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone length." << std::endl;
		return false;
	}

	length = (float)atof(str.c_str());

	return true;
}

static bool parseASFBoneAxis(std::string &line, size_t &nb_line, vec3 &axis)
{
	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for bone axis." << std::endl;
		return false;
	}

	size_t i = 0;
	float tab[3];

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

static bool parseASFBoneDof(std::string &line, size_t &nb_line, std::vector<std::string> &dof)
{
	trim(line);
	if (line.size() == 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": missing value for bone dof." << std::endl;
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

		if (std::find(dof.begin(), dof.end(), str) != dof.end())
		{
			std::cerr << "ASF input file error: line " << nb_line << ": bone dof element '" << str << "' already used." << std::endl;
			return false;
		}

		dof.push_back(str);

		trim(line);
	}

	return true;
}

static bool parseASFBoneLimit(std::string &line, size_t &nb_line, Limit &limit)
{
	trim(line);

	if (line[0] != '(')
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone limit." << std::endl;
		return false;
	}

	line = line.substr(1);
	for (size_t i = 0; i < 2; i++)
	{
		trim(line);
		size_t j = 0;
		std::string str;
		while (j != line.size() && line[j] != ' ' && line[j] != '\n' && line[j] != '\t' && line[j] != '\r' && line[j] != ')')
		{
			str += line[j];
			j++;
		}

		switch (i)
		{
			case 0:
				limit.min = (float)atof(str.c_str());
				break;

			case 1:
				limit.max = (float)atof(str.c_str());
				break;
			
			default:
				break;
		}

		line = line.substr(j);
	}

	trim(line);
	if (line[0] != ')')
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone limit." << std::endl;
		return false;
	}

	line = line.substr(1);
	trim(line);
	if (line.size() != 0)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone limit." << std::endl;
		return false;
	}

	return true;
}

bool InputParser::parseASFBoneLimits(std::string &line, size_t &nb_line, size_t dof_size, std::vector<Limit> &limits)
{
	Limit limit;

	trim(line);
	if (line.size() != 0)
	{
		if (!parseASFBoneLimit(line, nb_line, limit))
			return false;
		limits.push_back(limit);
		dof_size--;
	}

	nb_line++;
	for (size_t i = 0; i < dof_size && std::getline(this->_asf_file, line); i++)
	{
		if (!parseASFBoneLimit(line, nb_line, limit))
			return false;
		limits.push_back(limit);

		nb_line++;
	}
	
	return true;
}

bool InputParser::parseASFBone(size_t &nb_line)
{
	BoneDefinition bone;
	std::vector<std::string> dof;
	std::vector<Limit> limits;
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
		if (line[0] == '(')
		{
			std::cerr << "ASF input file error: line " << nb_line << ": invalid bone limit." << std::endl;
			return false;
		}

		std::string key = parseKey(line);
		if (key == ASF_KEY_BONEDATA_NAME)
		{
			if (is_name_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for bone name." << std::endl;
				return false;
			}
			if (!parseASFBoneName(line, nb_line, bone.name))
				return false;

			is_name_parsed = true;
		}
		else if (key == ASF_KEY_BONEDATA_DIRECTION)
		{
			if (is_direction_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for bone direction." << std::endl;
				return false;
			}
			if (!parseASFBoneDirection(line, nb_line, bone.direction))
				return false;

			is_direction_parsed = true;
		}
		else if (key == ASF_KEY_BONEDATA_LENGTH)
		{
			if (is_length_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for bone length." << std::endl;
				return false;
			}
			if (!parseASFBoneLength(line, nb_line, bone.length))
				return false;

			is_length_parsed = true;
		}
		else if (key == ASF_KEY_BONEDATA_AXIS)
		{
			if (is_axis_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for bone axis." << std::endl;
				return false;
			}
			if (!parseASFBoneAxis(line, nb_line, bone.axis))
				return false;

			is_axis_parsed = true;
		}
		else if (key == ASF_KEY_BONEDATA_DOF)
		{
			if (is_dof_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for bone dof." << std::endl;
				return false;
			}
			if (!parseASFBoneDof(line, nb_line, dof))
				return false;

			is_dof_parsed = true;
		}
		else if (key == ASF_KEY_BONEDATA_LIMITS)
		{
			if (is_limits_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": multiple definitions for bone limits." << std::endl;
				return false;
			}
			if (!is_dof_parsed)
			{
				std::cerr << "ASF input file error: line " << nb_line << ": invalid bone: found field 'limits' but missing field 'dof'." << std::endl;
				return false;
			}
			if (!parseASFBoneLimits(line, nb_line, dof.size(), limits))
				return false;

			is_limits_parsed = true;
		}
		else if (key == ASF_KEY_END)
			break;

		nb_line++;
	}

	nb_line++;
	if (!is_name_parsed)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone: missing field 'name'." << std::endl;
		return false;
	}
	if (!is_direction_parsed)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone: missing field 'direction'." << std::endl;
		return false;
	}
	if (!is_length_parsed)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone: missing field 'length'." << std::endl;
		return false;
	}
	if (!is_axis_parsed)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone: missing field 'axis'." << std::endl;
		return false;
	}
	if (is_dof_parsed && !is_limits_parsed)
	{
		std::cerr << "ASF input file error: line " << nb_line << ": invalid bone: found field 'dof' but missing field 'limits'." << std::endl;
		return false;
	}

	for (size_t i = 0; i < dof.size(); i++)
	{
		std::pair<std::string, Limit> pair(dof[i], limits[i]);
		bone.dof.insert(pair);
	}

	std::pair<std::string, BoneDefinition> pair(bone.name, bone);
	this->_bonedata.insert(pair);

	return true;
}

bool InputParser::parseASFBonedata(size_t &nb_line, std::string &next_key)
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
		{
			next_key = parseKey(line);
			break;
		}

		std::string key = parseKey(line);
		if (key == ASF_KEY_BEGIN)
		{
			if (!this->parseASFBone(nb_line))
				return false;
		}
		else
		{
			std::cerr << "ASF input file error: line " << nb_line << "unknown key '" << key << "'." << std::endl;
			return false;
		}
	}

	return true;
}



bool InputParser::parseASFHierarchy(size_t &nb_line)
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
		{
			std::cerr << "ASF input file error: line " << nb_line << ": missing 'end' key for hierarchy." << std::endl;
			return false;
		}

		std::string key = parseKey(line);
		if (key == ASF_KEY_BEGIN)
		{
			nb_line++;
			continue;
		}
		else if (key == ASF_KEY_END)
			break;
		else if (key != ASF_KEY_ROOT && this->_bonedata.find(key) == this->_bonedata.end())
		{
			std::cerr << "ASF input file error: line " << nb_line << ": unknown bone '" << key << "'." << std::endl;
			return false;
		}
		else
		{
			trim(line);
			while(line.size() != 0)
			{
				std::string next = parseKey(line);
				if ( this->_bonedata.find(next) == this->_bonedata.end())
				{
					std::cerr << "ASF input file error: line " << nb_line << ": unknown bone '" << next << "'." << std::endl;
					return false;
				}
				if (this->_hierarchy.find(next) != this->_hierarchy.end())
				{
					std::cerr << "ASF input file error: line " << nb_line << ": bone '" << next << "' already has a previous bone." << std::endl;
					return false;
				}
				
				std::pair<std::string, std::string> pair(next, key);
				this->_hierarchy.insert(pair);
				trim(line);
			}
		}
		nb_line++;
	}

	return true;
}



bool InputParser::parseASF(void)
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
	std::string next_key;
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
				if (!this->parseASFRoot(nb_line))
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
				if (!this->parseASFBonedata(nb_line, next_key))
				{
					this->_asf_file.close();
					return false;
				}
				if (next_key.size() != 0)
					key = next_key;

				is_bonedata_parsed = true;
			}
			if (key == ASF_KEY_HIERARCHY)
			{
				if (is_hierarchy_parsed)
				{
					std::cerr << "ASF input file error line " << nb_line << ": multiplie definitions for 'hierarchy'." << std::endl;
					this->_asf_file.close();
					return false;
				}
				if (!is_bonedata_parsed)
				{
					std::cerr << "ASF input file error line " << nb_line << ": found field 'hierarchy' but missing field 'bonedata'." << std::endl;
					return false;
				}
				if (!this->parseASFHierarchy(nb_line))
				{
					this->_asf_file.close();
					return false;
				}

				is_hierarchy_parsed = true;
			}
		}

		nb_line++;

		if (is_root_definition_parsed && is_bonedata_parsed && is_hierarchy_parsed)
			break;
	}

	if (!is_root_definition_parsed)
	{
		std::cerr << "ASF input file error: missing root definition." << std::endl;
		return false;
	}
	if (!is_bonedata_parsed)
	{
		std::cerr << "ASF input file error: missing bonedata definition." << std::endl;
		return false;
	}
	if (!is_hierarchy_parsed)
	{
		std::cerr << "ASF input file error: missing hierarchy definition." << std::endl;
		return false;
	}

	return true;
}

bool InputParser::parseAmc(void)
{


	return true;
}