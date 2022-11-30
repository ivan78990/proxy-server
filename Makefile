NAME	= proxy

SRC		=	srcs/main.cpp\
 			srcs/Server.cpp\
 			srcs/Utils.cpp\
 			srcs/Logger.cpp\
 			srcs/Poll.cpp\

HEADERS	=	includes/Logger.hpp \
			includes/Addition.hpp \
			includes/Server.hpp \


CXX		= c++

CPPFLAGS	= -g -Wall -Wextra -Werror

OBJ		= $(SRC:.cpp=.o)

$(NAME): $(OBJ) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) log

re: fclean all

.PHONY: all clean fclean