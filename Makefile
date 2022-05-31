NAME = snake

CC = g++

RM = rm -f

AR = ar rcs

SRCS = snake.cpp

OBJS = $(SRCS:.cpp=.o)

%.o : %.cpp
	$(CC) -c $<

$(NAME) : $(OBJS)
		$(CC) -o $(NAME) $(OBJS) -lncursesw

all : $(NAME)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all
