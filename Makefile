NAME = webserv

SRCS = main.cpp src/Config.cpp

FLAGS =  -std=c++11 -pedantic -Wall -Wextra -Werror -fsanitize=address -g 

COMP=clang++
ifeq ($(shell uname), Linux)
	COMP = g++ -DLINUX
endif

all : $(NAME)

$(NAME) :
	$(COMP) -o $(NAME) $(FLAGS) $(SRCS)

clean :
	rm -f *.hpp.gch
	rm -rf *.dSYM

fclean : clean
	rm -f $(NAME)
	rm -f *.out

re : fclean all

c : re
	clear
	./$(NAME) configs/default.conf 	

siege :
	siege -b -i -c 100 -f siege.urls