#include "../includes/ImguiValues.hpp"

ImguiValues::ImguiValues(void) : _length(1), _width(1), _prev_length(1), _prev_width(1) {}

ImguiValues::~ImguiValues(void) {}



float *ImguiValues::getLength(void)
{
	return &this->_length;
}

float *ImguiValues::getWidth(void)
{
	return &this->_width;
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
}