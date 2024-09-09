#ifndef ANIMATION_HPP
# define ANIMATION_HPP

# include <string>
# include <map>
# include <vector>

class Root;

typedef struct s_Frame
{
	unsigned int								pos;
	std::map<std::string, std::vector<float>>	bones;
}	Frame;

class Animation
{
	private:
		static bool	_playing;

		std::string			_name;
		std::vector<Frame>	_frames;
		Root *				_root;
		
		bool	_is_playing;
		size_t	_current_frame;

	public:
		Animation(std::string name);
		~Animation(void);

		void	addFrame(std::map<std::string, std::vector<float>> bones);

		void 	play(void);
		void	stop(void);

		std::string &			getName(void);
		size_t					getNumberFrames(void);
		std::vector<Frame> &	getFrames(void);

		void	setRoot(Root *root);
};

#endif