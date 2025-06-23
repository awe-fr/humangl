#include "../includes/ImguiValues.hpp"

ImguiValues::ImguiValues(void) : _length(1), _width(1), _prev_length(1), _prev_width(1), _head_size(1), _prev_head_size(1) {}

ImguiValues::~ImguiValues(void) {}

float *ImguiValues::getLength(void)
{
	return &this->_length;
}

float *ImguiValues::getWidth(void)
{
	return &this->_width;
}

float *ImguiValues::getHeadSize(void) {
	return &this->_head_size;
}

void ImguiValues::setAnimName(std::string new_name) {
	this->_anim_name = new_name;
}

void ImguiValues::notify(void)
{
	if (this->_prev_length != this->_length)
	{
		Param param;
		param.type = Length;
		param.value = this->_length;
		
		for (std::vector<IObserver *>::iterator it = this->_subscribers.begin(); it != this->_subscribers.end(); it++)
			(*it)->update(&param);

		this->_prev_length = this->_length;
	}
	
	if (this->_prev_width != this->_width)
	{
		Param param;
		param.type = Width;
		param.value = this->_width;

		for (std::vector<IObserver *>::iterator it = this->_subscribers.begin(); it != this->_subscribers.end(); it++)
			(*it)->update(&param);

		this->_prev_width = this->_width;
	}

	if (this->_prev_head_size != this->_head_size)
	{
		Param param;
		param.type = Head_size;
		param.value = this->_head_size;

		for (std::vector<IObserver *>::iterator it = this->_subscribers.begin(); it != this->_subscribers.end(); it++)
			(*it)->update(&param);

		this->_prev_head_size = this->_head_size;
	}

	if (this->_prev_anim_name != this->_anim_name)
	{
		Param param;
		param.type = Anim_name;
		param.name = this->_anim_name;

		for (std::vector<IObserver *>::iterator it = this->_subscribers.begin(); it != this->_subscribers.end(); it++)
			(*it)->update(&param);

		this->_prev_anim_name = this->_anim_name;
	}
}

void ImguiValues::setAnim(BVHAnimation *anim) {
	this->_actual_animation = anim;
	this->_prev_length = 1;
	this->_prev_width = 1;
	this->_prev_head_size = 1;
}

BVHAnimation *ImguiValues::getAnim(void) {
	return(this->_actual_animation);
}