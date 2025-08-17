SRC = builtins.c main.c utils.c
OBJ = $(SRC:.c=.o)

NAME = minishell
FLAGS = -Wall -Werror -Wextra -lreadline
CC = cc

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all