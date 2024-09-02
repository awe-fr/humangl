#ifndef MEMBER_HPP
# define MEMBER_HPP

# include <map>

# include "WindowApp.hpp"
# include "GraphicsMath.h"
# include "Root.hpp"
# include "InputParser.hpp"

class Member {
	private:
		std::string						_name;
		vec3							_direction;
		float							_length;
		vec3							_degree;
		std::map<std::string, Limit>	_degreeLock;
		Member *						_previous;
		Root *							_root;

		mat4 _model;

		GLfloat *_vertex;
		int *_index;

		GLuint _vao;
		GLuint _vbo;
		GLuint _ibo;
	public:
		Member(std::string name, vec3 direction, float length, vec3 degree, std::map<std::string, Limit> degree_lock, Member *previous, Root *root);
		~Member();
		void computeTravel();
		void setPrevious(Member *p);
		void setDegree(float x, float y, float z);
		GLuint getVAO();
		GLuint getVBO();
		GLuint getIBO();
		mat4 getModel();
		std::string getName();
};

#endif