#ifndef BVH_PARSER_HPP
# define BVH_PARSER_HPP

# include <string>
# include <vector>
# include <exception>
# include "GraphicsMath.h"
# include "WindowApp.hpp"
# include "Observer.hpp"
# include "ImguiValues.hpp"

# define MEMBER_BASE_WIDTH_START 0.05f
# define MEMBER_BASE_WIDTH_END 0.01f

class BVHAnimation
{
	public:
		class Member : public IObserver
		{
			public:
				enum Channel
				{
					XPOS,
					YPOS,
					ZPOS,
					XROT,
					YROT,
					ZROT
				};

			protected:
				const std::string		_name;
				vec3					_offset;
				vec3					_degree;
				float					_length;
				float					_length_ratio;
				std::vector<Channel>	_channels;
				std::vector<Member *>	_children;
				Member *				_parent;

				mat4					_model;

				GLfloat *				_vertex;
				int *					_index;

				GLuint 					_vao;
				GLuint 					_vbo;
				GLuint 					_ibo;

			public:
				// Constructor
				Member(std::string name, Member *parent) :
					_name(name), _parent(parent) {}
				~Member();
			
				// Getters
				const std::string		GetName(void) { return this->_name; }
				Member *				GetParent(void) { return this->_parent; }
				std::vector<Member *>	GetChildren(void) {return this->_children; }
				vec3					GetOffset(void) { return this->_offset; }
				float					GetLength(void) { return this->_length; }
				std::vector<Channel>	GetChannels(void) { return this->_channels; }
				size_t					GetChannelsNumber(void) {return this->_channels.size(); }
				
				// Setters
				void	SetOffset(vec3 offset) { this->_offset = offset; }
				void	SetLength(float length) { this->_length = length; this->_length_ratio = length;}
				void	SetChannels(std::vector<Channel> channels) { this->_channels = channels; }

				void	setRotX(float var) {this->_degree.x = var;};
				void	setRotY(float var) {this->_degree.y = var;};
				void	setRotZ(float var) {this->_degree.z = var;};
				
				void					computeTravel();

				GLuint 	getVAO();
				GLuint 	getVBO();
				GLuint 	getIBO();
				mat4 	getModel();

				void	AddChild(Member *child) { this->_children.push_back(child); }

				void update(void *param);
				void initGraphics(void);
		};

		class Root : public Member
		{
			private:
				vec3	_transDegree;

			public:
				Root(void) : Member("root", nullptr) {}

				void	setTransX(float var) {this->_transDegree.x = var;};
				void	setTransY(float var) {this->_transDegree.y = var;};
				void	setTransZ(float var) {this->_transDegree.z = var;};

				void initTrans(void) {this->_transDegree = {0,0,0};};
		};

		class Exception : public std::exception
		{
			private:
				const std::string _message;

			public:
				Exception(const std::string message) : _message(message) {}
				const char *what(void) { return this->_message.c_str(); }
		};

	private:
		const std::string				_file_path;
		const std::string				_name;
		Root							_root;
		std::vector<Member *>			_members;
		size_t							_nb_frames;
		float							_frame_time;
		std::vector<std::vector<float>>	_animation;

		size_t							_nb_line;
		bool							_is_parsed;

		// Exception messages
		const std::string	ExceptionHeader(void);
		const std::string	MissingKeyException(const std::string key);
		const std::string	CannotOpenFile(void);
		const std::string	MissingLeftBracket(void);
		const std::string	MissingRightBracket(void);
		const std::string	IncompleteFile(void);
		const std::string	InvalidFloat(void);
		const std::string	InvalidSizeT(void);
		const std::string	InvalidOffset(void);
		const std::string	InvalidChannelNumber(void);
		const std::string	InvalidChannelElement(void);
		const std::string	DuplicateChannelElement(void);
		const std::string	MissingChannelElement(size_t expected, size_t got);
		const std::string	MissingFrameElement(size_t expected, size_t got);
		const std::string	MissingNbFrames(size_t expected, size_t got);
		const std::string	NotParsed(void);


		void							ParseMember(std::ifstream &file, BVHAnimation::Member &member);
		vec3							ParseOffset(std::string line);
		std::vector<Member::Channel>	ParseChannels(std::string line);
		float							ParseFloat(std::string str);
		Member::Channel					GetChannel(std::string str);
		void							ParseBracket(std::ifstream &file, char bracket);
		size_t							ParseSizeT(std::string str);
		void							ParseAnimation(std::ifstream &file);
		void							InitMembersGraphic(void);

	public:
		// Constructor
		BVHAnimation(std::string file_path, std::string name) :
			_file_path(file_path), _name(name), _root(Root()), _is_parsed(false) {}

		// Getters
		const std::string						GetName(void) { return this->_name; }
		const std::string						GetFilePath(void) { return this->_file_path; }
		const size_t							GetNbFrames(void) { return this->_nb_frames; }
		const float								GetFrameTime(void) { return this->_frame_time; }
		const std::vector<std::vector<float>>	GetAnimation(void) { return this->_animation; }
		const size_t							GetTotalChannelsNumber(void);

		const Root								GetRoot(void) {return this->_root;}
		const std::vector<Member *>				GetMembers(void) {return this->_members;}

		void	Parse(void);
		void	Run(void);
};

#endif