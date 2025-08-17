#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>

typedef struct  s_env {
	char            *var;
	struct s_env   *next;
}                   t_env;

typedef struct s_redir {
	char *file;
	int type; // 0=input, 1=output, 2=append, 3=heredoc
	struct s_redir *next;
} t_redir;

typedef struct s_command {
	char **args;			// command + arguments
	t_redir *redir;			// linked list of redirections
	int pipe_out;			// 1 if output is piped to next command
	struct s_command *next;	// next command in pipeline
} t_command;

//main.c
void	ctrl_c(int s);
void	ft_lstfree(t_env **head);
char	*ft_lstgetvar(t_env *command, char *str);
char	*replace_variable(t_env *env_vars, char *input);
void	init(t_command **command, t_env **env_vars);

//builtins.c
int		equal_sign(char *str);
void	ft_lstaddback(t_env **head, char *str);
void	ft_lstremove(t_env **head, char *str);
void	ft_putstr(char *str, int nl);
void	ft_putstrs(char **strs, int nl);
void    env_printing(t_env *env_vars);
int		exp_equal(char *input);
int		exp_already(char *input, t_env *command);
void	bi_handler(t_command **command, t_env **env_vars);
int		bi_checker(char *command);

//utils.c
int		ft_lstcount(t_env *head);
int		ft_strcmp(char *s1, char *s2);
int		ft_strlen(char *str);
char	*ft_strdup(char *s);
int		ft_strchr(char *str, char c);
char	*ft_substr(char *s, int start, int len);
char	*ft_strjoin(char *s1, char *s2);
// static int	words(char *s, char c);
// static char	**freeing(char **m, int i);
// static char	*skip(char *s, char c);
char	**ft_split(char *s, char c);