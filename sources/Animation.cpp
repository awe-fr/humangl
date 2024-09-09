#include "../includes/Animation.hpp"
#include "../includes/Root.hpp"
#include "../includes/GraphicsMath.h"
#include "../includes/Singleton.hpp"

bool Animation::_playing = false;

Animation::Animation(std::string name) : _name(name),  _root(nullptr), _is_playing(false), _current_frame(0) {}

Animation::~Animation(void) {}



void Animation::addFrame(std::map<std::string, std::vector<float>> bones)
{
	Frame frame;
	frame.pos = this->_frames.size() + 1;
	frame.bones = bones;

	this->_frames.push_back(frame);
}



void Animation::play(void)
{
	if (Animation::_playing && !this->_is_playing)
		return;

	if (!this->_root)
	{
		std::cerr << "Animation '" << this->_name << "' error: root undefined." << std::endl;
		return;
	}

	else
	{
		Animation::_playing = true;
		this->_is_playing = true;
		MemberList *member_list = MemberList::getInstance();

		if (this->_current_frame == this->_frames.size())
			this->_current_frame = 0;
		
		Frame frame = this->_frames[this->_current_frame];

		for (std::map<std::string, std::vector<float>>::iterator it = frame.bones.begin(); it != frame.bones.end(); it++)
		{
			if (it->first == "root")
			{
				vec3 position;
				vec3 orientation;

				for (size_t i = 0; i < it->second.size(); i++)
				{
					switch (i)
					{
						case 0:
							position.x = it->second[i];
							break;

						case 1:
							position.y = it->second[i];
							break;

						case 2:
							position.z = it->second[i];
							break;

						case 3:
							orientation.x = it->second[i];
							break;

						case 4:
							orientation.y = it->second[i];
							break;

						case 5:
							orientation.z = it->second[i];
							break;
						
						default:
							break;
					}
				}

				this->_root->setPosition(position);
				this->_root->setOrientation(orientation);
			}
			else
			{
				Member *member = member_list->findByName(it->first);
				std::map<std::string, Limit> degree_lock = member->getDegreeLock();
				size_t i = 0;

				for (std::map<std::string, Limit>::iterator itt = degree_lock.begin(); itt != degree_lock.end(); itt++)
				{
					if (itt->first == "rx")
						member->setDegreeX(it->second[i]);
					else if (itt->first == "ry")
						member->setDegreeY(it->second[i]);
					else if (itt->first == "rz")
						member->setDegreeZ(it->second[i]);
					i++;
				}
			}
		}

		this->_current_frame++;
	}
}

void Animation::stop(void)
{
	if (this->_is_playing)
	{
		this->_is_playing = false;
		this->_current_frame = 0;
		Animation::_playing = false;
	}
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



void Animation::setRoot(Root *root)
{
	this->_root = root;
}