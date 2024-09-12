#include "../includes/Observer.hpp"
#include <algorithm>

IObserver::IObserver(void) : _subject(nullptr) {}

void IObserver::attach(ISubject *subject)
{
	if (this->_subject)
		this->_subject->detach(this);

	this->_subject = subject;
	this->_subject->attach(this);
}

void IObserver::detach(void)
{
	if (this->_subject)
		this->_subject->detach(this);
	
	this->_subject = nullptr;
}



void ISubject::attach(IObserver *subscriber)
{
	if (std::find(this->_subscribers.begin(), this->_subscribers.end(), subscriber) == this->_subscribers.end())
		this->_subscribers.push_back(subscriber);
}

void ISubject::detach(IObserver *subscriber)
{
	std::vector<IObserver *>::iterator it = std::find(this->_subscribers.begin(), this->_subscribers.end(), subscriber);

	if (it != this->_subscribers.end())
		this->_subscribers.erase(it);
}

void ISubject::notify(void)
{
	for (std::vector<IObserver *>::iterator it = this->_subscribers.begin(); it != this->_subscribers.end(); it++)
		(*it)->update(nullptr);
}