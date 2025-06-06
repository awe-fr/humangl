#ifndef WINDOWAPP_HPP
# define WINDOWAPP_HPP

# include <vector>
# include <sstream>
# include <fstream>
# include <iostream>

# include "./../imgui/imgui.h"
# include "./../imgui/imgui_impl_glfw.h"
# include "./../imgui/imgui_impl_opengl3.h"

# include <GL/glew.h>
# include <GLFW/glfw3.h>

# include "./GraphicsMath.h"

# define WIDTH 720
# define HEIGHT 480


class WindowsApp {
	private:
		GLFWwindow	*_window;
		GLuint		_programID;
		mat4		_projection;
		mat4		_view;
		double		_deltaTime;
	public:
		WindowsApp();
		~WindowsApp();

		void	computeMovement();
		void	deltaTime();
		double	getDeltaTime();
		mat4	getView();
		mat4	getProjection();
		GLFWwindow *getWindow();
		GLuint getProgramID();
		GLuint loadShaders(const char *vertexFilePath, const char *fragmentFilePath);
		bool isClosed();
};

#endif