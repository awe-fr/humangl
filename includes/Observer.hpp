#ifndef OBSERVER_HPP
# define OBSERVER_HPP

# include <vector>

class ISubject;

class IObserver
{
	private:
		ISubject *	_subject;

	public:
		IObserver(void);

		void 			attach(ISubject *subject);
		void			detach(void);
		virtual void	update(void *param) = 0;
};

class ISubject
{
	protected:
		std::vector<IObserver *>	_subscribers;

	public:
		void			attach(IObserver *subscriber);
		void			detach(IObserver *subscriber);
		virtual void	notify(void);
};

#endif