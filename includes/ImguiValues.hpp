#ifndef IMGUI_VALUES_HPP
# define IMGUI_VALUES_HPP

# include "Observer.hpp"

typedef enum e_ParamType
{
	Length,
	Width
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
		float	_prev_length;
		float	_prev_width;

	public:
		ImguiValues(void);
		~ImguiValues(void);

		float *	getLength(void);
		float *	getWidth(void);

		void	notify(void);
};


#endif