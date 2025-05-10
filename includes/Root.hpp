#ifndef ROOT_HPP
# define ROOT_HPP

# include <vector>
# include <string>
# include "WindowApp.hpp"
# include "GraphicsMath.h"

class Root
{
	private:
		std::vector<std::string>	_order;
		vec3						_postition;
		vec3						_orientation;

		mat4 _model;

		GLfloat *_vertex;
		int *_index;

		GLuint _vao;
		GLuint _vbo;
		GLuint _ibo;
	public:
		Root(std::vector<std::string> order);
		~Root(void);
		GLuint getVAO();
		GLuint getVBO();
		GLuint getIBO();
		mat4 getModel();

		void setPosition(vec3 new_position);
		void setOrientation(vec3 new_orientation);
};

#endif