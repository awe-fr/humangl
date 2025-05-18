#ifndef BVHPARSER_HPP
# define BVHPARSER_HPP

# include <string>
# include <fstream>
# include <iostream>
# include <vector>

# include "GraphicsMath.h"
# include "Root.hpp"
# include "Member.hpp"
# include "Singleton.hpp"

class BVHParser {
	private:
		std::string		_bvhPath;
		std::ifstream	_bvhFile;
		Root			*_root;
		std::vector<std::vector<float>> _frames;

		bool	parseHierarchy();
		bool	parseMotion();
		bool	parseRoot(std::string name);
		bool	parseMember(std::string name);
		bool	parseEndSite();

	public:
		BVHParser(std::string path);
		~BVHParser();

		void	changeAngle();
		bool	parseVBH();
};

#endif