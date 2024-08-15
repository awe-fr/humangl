NAME = humangl

SRCS = sources/main.cpp\
		sources/WindowsApp.cpp\

OBJS = $(SRCS:.cpp=.o)

CC = c++

CFLAGS = -Wall -Wextra -Werror -g

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
			
re:			fclean all

.PHONY:		all clean fclean re