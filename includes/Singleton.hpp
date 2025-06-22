#ifndef SINGLETON_HPP
# define SINGLETON_HPP

# include <mutex>

# include "BVHAnimation.hpp"

template <typename T>
class Singleton {
	protected:
		static T *_instance;
		static std::mutex _mutex;

		Singleton() {};
		virtual ~Singleton() {};
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
	public:
		static T *getInstance() {
			std::lock_guard<std::mutex> lock(_mutex);
			if (_instance == nullptr)
				_instance = new T();
			return (_instance);
		}
		static void deleteInstance() {
			delete _instance;
			_instance = nullptr;
		}
};

template <typename T>
T* Singleton<T>::_instance = nullptr;

template <typename T>
std::mutex Singleton<T>::_mutex;

template <typename L>
class List {
	protected:
		std::vector<L *> _list;
	public:
		std::vector<L *> getList() {return this->_list;};
		void add(L *toAdd) {
			if (toAdd != nullptr)
				this->_list.push_back(toAdd);
		};
		void remove(L *toRm) {
			if (toRm != nullptr) {
				for (int i = this->_list.size(); i > 0; i--) {
					if (this->_list[i - 1] == toRm) {
						this->_list.erase(this->_list.begin() + i - 1);
						break;
					}
				}
			}
		};
		void cleanup() {
			this->_list.clear();
		};
};

class MemberList : public Singleton<MemberList>, public List<BVHAnimation::Member>{
	private:
		MemberList() {};
		~MemberList() {};
		MemberList(const MemberList&) = delete;
		MemberList& operator=(const MemberList&) = delete;

		friend Singleton;

	public:
		BVHAnimation::Member *findByName(std::string name)
		{
			for (std::vector<BVHAnimation::Member *>::iterator it = this->_list.begin(); it != this->_list.end(); it++)
			{
				if ((*it)->GetName() == name)
					return *it;
			}

			return nullptr;
		}
};

class AnimationList : public Singleton<AnimationList>, public List<BVHAnimation>{
	private:
		AnimationList() {};
		~AnimationList() {};
		AnimationList(const AnimationList&) = delete;
		AnimationList& operator=(const AnimationList&) = delete;

		friend Singleton;

	public:
		BVHAnimation *findByName(std::string name)
		{
			for (std::vector<BVHAnimation *>::iterator it = this->_list.begin(); it != this->_list.end(); it++)
			{
				if ((*it)->GetName() == name)
					return *it;
			}

			return nullptr;
		}
};

#endif