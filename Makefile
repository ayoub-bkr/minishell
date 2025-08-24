SRC = main.c parsing.c \
	execution/builtins_env.c \
	execution/builtins_expo.c \
	execution/builtins_main.c \
	execution/heredoc.c \
	execution/pipeline.c \
	execution/redirection.c \
	execution/utils_1.c \
	execution/utils_2.c \
	execution/utils_3.c
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