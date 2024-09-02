#ifndef ROOT_HPP
# define ROOT_HPP

# include <vector>
# include <string>
# include "GraphicsMath.h"

class Root
{
	private:
		std::vector<std::string>	_order;
		vec3						_postition;
		vec3						_orientaion;

	public:
		Root(std::vector<std::string> order, vec3 position, vec3 orientation);
		~Root(void);
};

#endif