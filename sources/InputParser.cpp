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
		}

		nb_line++;
	}

	return true;
}

bool InputParser::parseAmc(void)
{


	return true;
}