NAME = minishell
SRC = main.c \
	  src/parsing.c  \
	  src/ad_print.c  \
	  src/printing.c  \
	  src/utils/tokenizing.c  \
	  src/utils/token_utils.c  \
	  src/utils/return_token_type.c \
	  src/utils/new_t_string.c \
	  src/utils/tokanize_word.c \
	  src/utils/create_word_token.c \
	  src/utils/handle_delimiters.c \
	  src/utils/set_struct_defaults.c \
	  src/utils/create_token_node.c \
	  src/libft/ft_memcpy.c \
	  src/libft/is_delimiter.c
CC = gcc
CFLAGS = -g3 -Wall -Wextra -Werror #-fsanitize=address
LIBS = -lreadline

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LIBS)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
