#ifndef ANIMATION_HPP
# define ANIMATION_HPP

# include <string>
# include <map>
# include <vector>

typedef struct s_Frame
{
	unsigned int								pos;
	std::map<std::string, std::vector<float>>	bones;
}	Frame;

class Animation
{
	private:
		std::string			_name;
		std::vector<Frame>	_frames;

	public:
		Animation(std::string name);
		~Animation(void);

		void	addFrame(std::map<std::string, std::vector<float>> bones);

		std::string &	getName(void);
		size_t			getNumberFrames(void);
};

#endif