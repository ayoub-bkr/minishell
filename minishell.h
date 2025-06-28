#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

typedef struct  s_data {
    char            *varenv;
    struct s_data   *next;
}                   t_data;

void    cmd_env(t_data *cmds);
void    ft_lstaddback(t_data **head, char *var);
int	ft_strncmp(char *s1, char *s2, int n);
void	ft_putstr(char *str);
void    echo(char *str);
