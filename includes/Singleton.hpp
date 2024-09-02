#ifndef SINGLETON_HPP
# define SINGLETON_HPP

# include <mutex>

# include "Member.hpp"

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
			for (int i = this->_list.size(); i > 0; i--)
				delete this->_list[i - 1];
		};
};

class MemberList : public Singleton<MemberList>, public List<Member>{
	private:
		MemberList() {};
		~MemberList() {};
		MemberList(const MemberList&) = delete;
		MemberList& operator=(const MemberList&) = delete;

		friend Singleton;

	public:
		Member *findByName(std::string name)
		{
			for (std::vector<Member *>::iterator it = this->_list.begin(); it != this->_list.end(); it++)
			{
				if ((*it)->getName() == name)
					return *it;
			}

			return nullptr;
		}
};

#endif