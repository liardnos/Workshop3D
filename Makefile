SRC = 	main.c \
		mat4x4.c 


OBJ = $(SRC:.c=.o)

NAME = a.out

CC = gcc -g

LIB = -l m -l csfml-graphics -l csfml-window -g

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