#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

typedef struct  s_line {
    char    *data;
    struct s_line   *next;
}           t_line;

int	ft_strcmp(char *s1, char *s2);
void    echo(char *str);
