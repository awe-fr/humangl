#ifndef INPUT_PARSER_HPP
# define INPUT_PARSER_HPP

# include <string>
# include <fstream>
# include <iostream>
# include <vector>
# include <algorithm>
# include <stdlib.h>
# include <map>

# include "GraphicsMath.h"

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

class Root;

typedef struct s_RootDefinition
{
	std::vector<std::string>	order;
	std::string					axis;
	vec3						position;
	vec3						orientation;
}	RootDefinition;

typedef struct s_Limit
{
	float	min;
	float	max;
}	Limit;

typedef struct s_BoneDefinition
{
	std::string						name;
	vec3							direction;
	float							length;
	vec3							axis;
	std::map<std::string, Limit>	dof;

}	BoneDefinition;

class InputParser
{
	private:
		std::string		_asf_path;
		std::string		_amc_path;
		std::ifstream	_asf_file;
		std::ifstream	_amc_file;

		RootDefinition							_root_definition;
		Root *									_root;
		std::map<std::string, BoneDefinition>	_bonedata;
		std::map<std::string, std::string>		_hierarchy;
		std::vector<std::string>				_hierarchy_order;

		bool	parseASFRoot(size_t &nb_line);
		bool	parseASFRootOrder(std::string &line, size_t &nb_line);
		bool	parseASFRootAxis(std::string &line, size_t &nb_line);
		bool	parseASFRootPosition(std::string &line, size_t &nb_line);
		bool	parseASFRootOrientation(std::string &line, size_t &nb_line);

		bool	parseASFBonedata(size_t &nb_line, std::string &next_key);
		bool	parseASFBone(size_t &nb_line);
		bool	parseASFBoneName(std::string &line, size_t &nb_line, std::string &name);
		bool	parseASFBoneLimits(std::string &line, size_t &nb_line, size_t dof_size, std::vector<Limit> &limits);

		bool	parseASFHierarchy(size_t &nb_line);

		void	buildRoot(void);
	
	public:
		InputParser(std::string asf_path, std::string amc_path);
		~InputParser(void);

		bool	parseASF(void);
		bool	parseAmc(void);

		void	buildMembers(void);

		Root *	getRoot(void);
};

#endif