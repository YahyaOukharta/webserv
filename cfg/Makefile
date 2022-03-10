NAME = cfg

SRCS = *.cpp

FLAGS = -Wall -Wextra -Werror -fsanitize=address -g

all: $(NAME)

$(NAME) :
	clang++ $(FLAGS) $(SRCS) -o $(NAME)

clean:
	rm -rf .DSYM*

fclean: clean
	rm -rf $(NAME)

re :fclean all