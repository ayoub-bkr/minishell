#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>

typedef struct  s_data {
	char            *varenv;
	struct s_data   *next;
}                   t_data;

//main.c
int		exp_equal(char *input);
int		exp_already(char *input, t_data *cmds);

//builtins.c
int		equal_sign(char *str);
void    cmd_env(t_data *cmds);
void    ft_lstaddback(t_data **head, char *var);
void	ft_lstremove(t_data **head, char *str);

//utils.c
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(char *s1, char *s2, int n);
void	ft_putstr(char *str);
void	ft_putstr_nl(char *str);
char	*ft_strdup(char *str);
int		ft_strchr(char *str, char c);
char	*ft_substr(char *s, unsigned int start, size_t len);