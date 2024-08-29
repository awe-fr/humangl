#ifndef INPUT_PARSER_HPP
# define INPUT_PARSER_HPP

# include <string>
# include <fstream>
# include <iostream>
# include <vector>
# include <algorithm>
# include <stdlib.h>

# include "./../includes/GraphicsMath.h"

# define ASF_KEY_ROOT "root"
# define ASF_KEY_BONEDATA "bonedata"
# define ASF_KEY_HIERARCHY "hierarchy"
# define ASF_KEY_BEGIN "begin"
# define ASF_KEY_END "end" 

# define ASF_KEY_ROOT_ORDER "order"
# define ASF_KEY_ROOT_AXIS "axis"
# define ASF_KEY_ROOT_POSITION "position"
# define ASF_KEY_ROOT_ORIENTATION "orientation"
# define ASF_KEY_BONEDATA_NAME "name"
# define ASF_KEY_BONEDATA_DIRECTION "direction"
# define ASF_KEY_BONEDATA_LENGTH "length"
# define ASF_KEY_BONEDATA_AXIS "axis"
# define ASF_KEY_BONEDATA_DOF "dof"
# define ASF_KEY_BONEDATA_LIMITS "limits"

typedef struct s_RootDefinition
{
	std::vector<std::string>	order;
	std::string					axis;
	vec3						position;
	vec3						orientation;
}	RootDefinition;

class InputParser
{
	private:
		std::string		_asf_path;
		std::string		_amc_path;
		std::ifstream	_asf_file;
		std::ifstream	_amc_file;

		RootDefinition	_root_definition;

		bool	parseAsfRoot(size_t &nb_line);
		bool	parseAsfRootOrder(std::string &line, size_t &nb_line);
		bool	parseAsfRootAxis(std::string &line, size_t &nb_line);
		bool	parseAsfRootPosition(std::string &line, size_t &nb_line);
		bool	parseAsfRootOrientation(std::string &line, size_t &nb_line);

	public:
		InputParser(std::string asf_path, std::string amc_path);
		~InputParser(void);

		bool	parseAsf(void);
		bool	parseAmc(void);

};

#endif