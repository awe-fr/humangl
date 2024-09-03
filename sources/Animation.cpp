#include "../includes/Animation.hpp"

Animation::Animation(std::string name) : _name(name) {}

Animation::~Animation(void) {}



void Animation::addFrame(std::map<std::string, std::vector<float>> bones)
{
	Frame frame;
	frame.pos = this->_frames.size() + 1;
	frame.bones = bones;

	this->_frames.push_back(frame);
}



std::string &Animation::getName(void)
{
	return this->_name;
}

size_t Animation::getNumberFrames(void)
{
	return this->_frames.size();
}

std::vector<Frame> &Animation::getFrames(void)
{
	return this->_frames;
}