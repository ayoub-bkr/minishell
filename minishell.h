#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

extern int				g_exit_status;

typedef struct s_env
{
	char				*var;
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_redir
{
	char				*file;
	int					type;
	int					heredoc_fd;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*redir;
	int					pipe_out;
	struct s_command	*next;
}						t_command;

typedef struct s_gc
{
	void				*ptr;
	struct s_gc			*next;
}						t_gc;

//-----------------------------

typedef enum s_token_type
{
	T_WORD,
	T_PIPE,
	T_RED_IN,
	T_RED_OUT,
	T_APPEND,
	T_HEREDC
}						t_token_type;

typedef struct s_token
{
	char				*str;
	t_token_type		type;
}						t_token;

typedef struct list
{
	t_token				*token;
	struct list			*next;
}						t_list;

typedef enum s_quote
{
	UNQUOTED,
	IN_SINGLE,
	IN_DOUBLE
}						t_quote_state;

typedef struct string
{
	char				*str;
	char				del;
	int					len;
	int					cap;
	int					start;
	int					end;
	int					peek;
	char				quote;
	int					q_pos;
	int					w_q_len;
}						t_string;

//------------------- execution -------------------

// builtins_cd.c
int						expo_valid_id(char *input);
char					*bi_cd_home(t_env *env_vars, char *str);
void					bi_cd(char **args, t_env *env_vars);

// builtins_env.c
void					env_printing(t_env *env_vars, int expo);
void					env_lstremove(t_env **head, char **cmds);
void					env_lstaddback(t_env **head, char *str);
int						env_lstcount(t_env *head);
char					**env_filling(t_env *head);

// builtins_expo.c
int						expo_equal(char *input);
int						expo_already(char *input, t_env *command);
void					expo_lstedit(char *input, t_env **env_vars);
void					expo_env_init(t_command **command, t_env **env_vars,
							int i);
void					expo_handler(t_command **command, t_env **env_vars);

// builtins_main.c
void					bi_pwd(void);
void					bi_exit(char **args);
void					bi_echo(char **args);
int						bi_handler(t_command **command, t_env **env_vars);
int						bi_checker(char *command);

// extern_cmd.c
void					exiting(int status);
char					*get_path(t_env *env_vars, char *cmd);
void					ext_child(t_command *command, t_env *env_vars);
int						ext_handler(t_command *command, t_env *env_vars);

// heredoc.c
void					heredoc(t_redir *redir);
void					heredoc_init(t_command *command);

// pipeline.c
void					pipe_ext_handler(t_command *command, t_env *env_vars);
void					pipe_waiting(pid_t pid);
void					pipe_setup(t_command *cmd, t_env **env_vars,
							int *fd, int *prev_fd);
pid_t					pipe_forking(t_command *cmd, t_env **env_vars,
							int *fd, int *prev_fd);
void					piping(t_command *cmd, t_env **env_vars);

// redirection.c
void					red_input(t_redir *redir);
void					red_output(t_redir *redir);
void					red_append(t_redir *redir);
void					redirecting(t_redir *redir);

// utils_1.c
int						ft_strcmp(char *s1, char *s2);
int						ft_strlen(char *str);
char					*ft_strdup(char *s);
int						ft_strchr(char *str, char c);

// utils_2.c
char					*ft_substr(char *s, int start, int len);
char					*ft_strjoin(char *s1, char *s2);
int						split_words(char *s, char c);
char					**split_freeing(char **m, int i);
char					**ft_split(char *s, char c);

// utils_3.c
int						ft_isdigit(char *str);
int						ft_isalpha(int c);
void					ft_putstr(char *str, int nl);
void					ft_putstrs(char **strs, int nl);

// gc.c
void					ft_bzero(void *s, size_t n);
void					*gc_calloc(int size);

// main.c
void					ctrl_c(int s);
void					executing(t_command **command, t_env **env_vars);
void					minishell_main(t_command **command, t_env **env_vars,
							t_list **head);
void					anti_norm(t_command **command, t_env **env_vars,
							t_list **head, char **envp);

//-------------------- parsing --------------------

void					create_key_vaue(t_env *single_var);
void					create_key_value_pairs(t_env *env_vars);
char					*get_key(char *s);
char					*get_value(char *s);
int						init(t_list **head, t_env *env_vars);
void					*ft_memcpy(void *dest, void *src, int n);
t_token					*ft_newtoken(char *s, t_token_type type);
t_list					*ft_lstnew(t_token *token);
void					ft_lstadd_back(t_list **head, t_list *new);
t_list					*ft_lstlast(t_list *lst);
t_token_type			return_token_type(const char *s);
t_string				*ft_newstr(char *s);
void					tokenize(char *str, t_list **head);
bool					tokenize_metachar(char **str, t_list **head);
char					*extract_word(char **p, int size);
void					add_token(t_list **head, char *str, t_token_type type);
bool					tokenize_word(char **p, t_list **head);
void					cmd_lstaddback(t_command **head, t_command *new);
t_redir					*redir_new(char *file, int type);
void					redir_addback(t_redir **head, t_redir *new);
void					parsing(t_command **command, t_list *head);
char					*get_env_value(char *var_name, t_env *env_vars);
void					string_append_str(t_string *dest, char *src);
t_string				*string_init(int initial_cap);
void					process_token_expansion(t_token *token,
							t_env *env_vars);
int						quote_syntax_error(t_list *head);
void					handle_redirection(t_list **cur, t_command *cmd,
							int type);
void					string_append_char(t_string *str, char c);
void					handle_quote(char quote, char *current_quote);
void					handle_dollar_sign(char *original, int *i,
							char current_quote, t_string *result,
							t_env *env_vars);