SRC = builtins_1.c builtins_2.c builtins_3.c main.c parsing.c utils_1.c utils_2.c
OBJ = $(SRC:.c=.o)

NAME = minishell
FLAGS = -Wall -Werror -Wextra
CC = cc

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) -lreadline

all: $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all