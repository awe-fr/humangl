#ifndef IMGUI_VALUES_HPP
# define IMGUI_VALUES_HPP

# include "Observer.hpp"

typedef enum e_ParamType
{
	Length,
	Width,
	Head_size
}	ParamType;

typedef struct s_Param
{
	ParamType	type;
	float		value;
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

	public:
		ImguiValues(void);
		~ImguiValues(void);

		float *	getLength(void);
		float *	getWidth(void);
		float *	getHeadSize(void);

		void	notify(void);
};


#endif