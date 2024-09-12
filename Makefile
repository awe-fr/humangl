NAME = humangl

SRCS = sources/main.cpp\
		sources/WindowApp.cpp\
		sources/GraphicsMath.cpp\
		sources/Member.cpp\
		sources/InputParser.cpp\
		sources/Root.cpp\
		sources/Animation.cpp\
		sources/Observer.cpp\
		sources/ImguiValues.cpp\
		imgui/imgui.cpp\
		imgui/imgui_demo.cpp\
		imgui/imgui_draw.cpp\
		imgui/imgui_impl_glfw.cpp\
		imgui/imgui_impl_opengl3.cpp\
		imgui/imgui_tables.cpp\
		imgui/imgui_widgets.cpp\

OBJS = $(SRCS:.cpp=.o)

CC = c++

CFLAGS = -g

LDFLAGS = -Iinclude -Llib -lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -ldl -D GLEW_STATIC

RM = rm -f

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

%.o: 			%.cpp
				$(CC) $(CFLAGS) -c $< -o $@
				
clean:
			$(RM) $(OBJS)
			
fclean:		clean
			$(RM) $(NAME)
			$(RM) imgui.ini
			
re:			fclean all

.PHONY:		all clean fclean re