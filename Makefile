NAME = snake

CC = g++

RM = rm -f

AR = ar rcs

SRCS = snake.cpp

OBJS = $(SRCS:.cpp=.o)

%.o : %.cpp
	$(CC) $< -lncursesw

$(NAME) : $(OBJS)
	    @ $(AR) $(NAME) $(OBJS)

all : $(NAME)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all
