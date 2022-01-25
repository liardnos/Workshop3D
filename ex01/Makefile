SRC = 	main.c \
		mat4x4.c \
		utils.c \


OBJ = $(SRC:.c=.o)

NAME = a.out

FLAGS = -Wall -Wextra

CC = gcc -g $(FLAGS)

LIB = -l m -l csfml-graphics -l csfml-window -g $(FLAGS)

all: $(NAME)

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ) $(LIB)

run: clear all
	./$(NAME)

valgrind: clear all
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=definite ./$(NAME)

re: clear clean all

clear:
	clear

clean:
	rm -f $(OBJ)
	rm -f a.out