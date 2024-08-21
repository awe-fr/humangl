#ifndef HEADER_H
# define HEADER_H

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <cmath>

# include <GL/glew.h>
# include <GLFW/glfw3.h>

# define HEIGHT 720
# define WIDTH 1280

# include "./GraphicsMath.h"
# include "./WindowsApp.hpp"

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

mat4 movement(GLFWwindow *window);

void getDeltaTime(void);

#endif