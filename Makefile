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
	parsing/src/parsing.c  \
	parsing/main.c \
	  parsing/src/expansion/mark_quotes.c  \
	  parsing/src/ad_print.c  \
	  parsing/src/printing.c  \
	  parsing/src/utils/create_key_value.c  \
	  parsing/src/utils/tokenizing.c  \
	  parsing/src/utils/token_utils.c  \
	  parsing/src/utils/return_token_type.c \
	  parsing/src/utils/new_t_string.c \
	  parsing/src/utils/tokanize_word.c \
	  parsing/src/utils/create_word_token.c \
	  parsing/src/utils/handle_delimiters.c \
	  parsing/src/utils/set_struct_defaults.c \
	  parsing/src/utils/create_token_node.c \
	  parsing/src/libft/ft_memcpy.c \
	  parsing/src/libft/is_delimiter.c

OBJ = $(SRC:.c=.o)

NAME = minishell
FLAGS = -g3 -Wall -Werror -Wextra
FLAGS += -fsigned-char
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