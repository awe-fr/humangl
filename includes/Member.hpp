#ifndef MEMBER_HPP
# define MEMBER_HPP

# include "./WindowApp.hpp"
# include "./GraphicsMath.h"

class Member {
	private:
		std::string _name;
		Member *_previous;
		Member *_next;
		float _length;
		vec3 _degreeLock;

		mat4 _model;
		vec3 _degree;

		GLfloat *_vertex;
		int *_index;

		GLuint _vao;
		GLuint _vbo;
		GLuint _ibo;
	public:
		Member(std::string name, float length);
		~Member();
		void addNext(Member *n);
		void addPrevious(Member *p);
		void setDegree(float x, float y, float z);
		GLuint getVAO();
		GLuint getVBO();
		GLuint getIBO();
		mat4 getModel();
};

#endif