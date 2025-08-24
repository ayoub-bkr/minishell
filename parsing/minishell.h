#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <stdbool.h>


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

#endif
