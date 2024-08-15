#ifndef WINDOWSAPP_HPP
# define WINDOWSAPP_HPP

# include "./header.h"

class WindowsApp {
	private:
		GLFWwindow	*_window;
	public:
		WindowsApp();
		~WindowsApp();

		bool isClosed();
};

#endif