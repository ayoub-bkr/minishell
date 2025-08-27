SRC = main.c gc.c\
	execution/builtins_cd.c \
	execution/builtins_env.c \
	execution/builtins_expo.c \
	execution/builtins_main.c \
	execution/extern_cmd.c \
	execution/heredoc.c \
	execution/pipeline.c \
	execution/redirection.c \
	execution/utils_1.c \
	execution/utils_2.c \
	execution/utils_3.c \
	parsing/init.c \
	parsing/parsing.c \
	parsing/pars_redirection.c \
	  parsing/src/expansion/expansion_handle.c  \
	  parsing/src/expansion/mark_quotes.c  \
	  parsing/src/expansion/quote_syntax_error.c  \
	  parsing/src/expansion/token_expansion.c  \
	  parsing/src/utils/create_key_value.c  \
	  parsing/src/utils/token_utils.c  \
	  parsing/src/utils/tokenizing.c

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