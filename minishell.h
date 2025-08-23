#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>



typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_redir
{
	char			*file;
	int				type; // 0=input, 1=output, 2=append, 3=heredoc
	int				heredoc_fd;
	struct s_redir	*next;
}					t_redir;

typedef struct s_command
{
	char				**args;		// command + arguments
	t_redir				*redir;		// linked list of redirections
	int					pipe_out;	// 1 if output is piped to next command
	struct s_command	*next;		// next command in pipeline
}						t_command;




//parsing
typedef enum
{
	T_WORD,
	T_PIPE,
	T_RED_IN,
	T_RED_OUT,
	T_APPEND,
	T_HEREDOC
} TokenType;

typedef struct
{
	char *str;
	TokenType type;
} Token;

typedef struct list
{
	Token 		*token;
	struct list	*next;
}	t_list;

void debug_print_all(t_list *tokens, t_command *commands);

typedef struct
{
	char *str;
	char del;
	int len;
	int cap;
	int start;
	int end;
	int peek;
	char quote;
	int q_pos;
	int w_q_len;
} t_string;

void init(t_list **head);
void *ft_memcpy(void *dest, void *src, int n);
Token *ft_newtoken(char *s, TokenType type);
t_list *ft_lstnew(Token *token);
void	ft_lstadd_back(t_list **head, t_list *new);
t_list *ft_lstlast(t_list *lst);
TokenType return_token_type(const char *s);
int is_delimiter(char c);
TokenType return_token_type(const char *s);
Token *ft_newtoken(char *s, TokenType type);
Token *ft_newtoken(char *s, TokenType type);
t_list *ft_lstnew(Token *token);
t_list *ft_lstlast(t_list *lst);
void ft_lstadd_back(t_list **head, t_list *new);
t_string *ft_newstr(char *s);
char *tokanize_word(t_string *str);
char *create_quote_word(t_string *str);
void set_def(t_string *str);
void create_token_node(t_list **head, char *s);
char	*collect_delimiter(t_string *str);
void handle_delimiter(t_string *str, t_list **head);
// tokenizing
void tokenize(char *str, t_list **head);
bool tokenize_metachar(char **str, t_list **head);
char *extract_word(char **p, int size);
void add_token(t_list **head, char *str, TokenType type);
bool tokenize_word(char **p, t_list **head);
// from ayoub
void		cmd_lstaddback(t_command **head, t_command *new);
t_redir		*redir_new(char *file, int type);
void		redir_addback(t_redir **head, t_redir *new);
// parsing function
t_command *parsing(t_command **command, t_list *head);
void print_list(t_list *head);
void print_command(t_command *head);








//builtins_expo.c
int			expo_valid_id(char *input);
int			expo_equal(char *input);
int			expo_already(char *input, t_env *command);
void		expo_lstedit(char *input, t_env **env_vars);
void		expo_handler(t_command **command, t_env **env_vars);

//builtins_env.c
void		env_printing(t_env *env_vars, int expo);
void		env_lstremove(t_env **head, char **cmds);
void		env_lstaddback(t_env **head, char *str);
int			env_lstcount(t_env *head);
char		**env_filling(t_env *head);

//builtins_main.c
void		bi_pwd();
char		*bi_cd_home(t_env *env_vars, char *str);
void		bi_cd(char **args, t_env *env_vars);
void		bi_handler(t_command **command, t_env **env_vars);
int			bi_checker(char *command);

//main.c
void		ext_handler(t_command *command, t_env *env_vars);
void		ctrl_c(int s);
void		cmd_freeing(t_command **command);

//heredoc.c
void		heredoc_init(t_command *command);
void		heredoc(t_redir *redir);

//pipeline.c
void		pipe_ext_handler(t_command *command, t_env *env_vars);
void		pipe_setup(t_command *cmd, int *fd, int *prev_fd);
void		pipe_handler(t_command *cmd, t_env **env_vars, int *fd, int *prev_fd);
void		piping(t_command *cmd, t_env **env_vars);

//redirection.c
void    	red_input(t_redir *redir);
void		red_output(t_redir *redir);
void		red_append(t_redir *redir);
void		redirecting(t_redir *redir);

//parsing.c
// void		cmd_lstaddback(t_command **head, t_command *new);
// int			count_tokens(char **tokens);
// t_redir		*redir_new(char *file, int type);
// void		redir_addback(t_redir **head, t_redir *new);
// t_command	*parsing(char *cmd_str);
// char		*ft_lstgetvar(t_env *command, char *str);
// char		*replace_variable(t_env *env_vars, char *input);
// void		ft_lstfree(t_env **head);
// void		init(t_command **command, t_env **env_vars);

//utils_1.c
int			ft_strcmp(char *s1, char *s2);
int			ft_strlen(char *str);
char		*ft_strdup(char *s);
int			ft_strchr(char *str, char c);

//utils_2.c
char		*ft_substr(char *s, int start, int len);
char		*ft_strjoin(char *s1, char *s2);
int			split_words(char *s, char c);
char		**split_freeing(char **m, int i);
char		**ft_split(char *s, char c);

//utils_3.c
int			ft_isdigit(int c);
int			ft_isalpha(int c);
void		ft_putstr(char *str, int nl);
void		ft_putstrs(char **strs, int nl);
