#ifndef IMGUI_VALUES_HPP
# define IMGUI_VALUES_HPP

# include "Observer.hpp"
# include "BVHAnimation.hpp"

class BVHAnimation;

typedef enum e_ParamType
{
	Length,
	Width,
	Head_size,
	Anim_name
}	ParamType;

typedef struct s_Param
{
	ParamType	type;
	float		value;
	std::string name;
}	Param;

class ImguiValues : public ISubject
{
	private:
		float	_length;
		float	_width;
		float	_head_size;
		float	_prev_length;
		float	_prev_width;
		float	_prev_head_size;
		std::string	_anim_name;
		std::string	_prev_anim_name;

		BVHAnimation *_actual_animation;

	public:
		ImguiValues(void);
		~ImguiValues(void);

		float *	getLength(void);
		float *	getWidth(void);
		float *	getHeadSize(void);
		
		void	setAnimName(std::string new_name);

		void	notify(void);

		void	setAnim(BVHAnimation *anim);
		BVHAnimation	*getAnim(void);
};


#endif