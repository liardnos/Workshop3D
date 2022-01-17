SRC = 	main.c \
		mat4x4.c 


OBJ = $(SRC:.c=.o)

NAME = a.out

LIB = -l m -l csfml-graphics -l csfml-window

all: $(NAME)

$(NAME): $(OBJ)
	gcc -o $(NAME) $(OBJ)

run: all
	./$(NAME)

valgrind: all
	./$(NAME)

re:

clean:
	rm -f $(OBJ)
	rm -f a.out