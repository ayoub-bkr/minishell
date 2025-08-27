#include "../minishell.h"

int	pipe_syntax_error(t_list *head)
{
	t_list			*cur;
	int				i;
	t_token_type	prev_token_type;

	if (!head)
		return (0);
	cur = head;
	i = 0;
	while (cur)
	{
		if (cur->token->type == T_PIPE)
		{
			if (i == 0)
				return (printf("[+] syntax error\n{-} Invalid start\n"), 0);
			if (prev_token_type == T_PIPE)
				return (printf("[+] syntax error\n{-} pipes Error\n"), 0);
			if (!cur->next || cur->next->token->type != T_WORD)
				return (printf("[+] syntax error\n{-} invalid command\n"), 0);
		}
		prev_token_type = cur->token->type;
		i++;
		cur = cur->next;
	}
	return (1);
}

int	redir_syntax_error(t_list *head)
{
	t_list			*cur;
	t_token_type	pttype;
	t_token_type	cttype;

	if (!head)
		return (0);
	cur = head;
	pttype = T_WORD;
	while (cur)
	{
		cttype = cur->token->type;
		if (cttype == T_RED_OUT || cttype == T_RED_IN || cttype == T_APPEND
			|| cttype == T_HEREDC)
		{
			if (pttype == T_RED_OUT || pttype == T_RED_IN || pttype == T_APPEND
				|| pttype == T_HEREDC)
				return (printf("[+] syntax error\n{-} redirections\n"), 0);
			if (!cur->next || cur->next->token->type != T_WORD)
				return (printf("[+] syntax error\n{-} redirections\n"), 0);
		}
		pttype = cur->token->type;
		cur = cur->next;
	}
	return (1);
}

int	syntax_error(t_list *head)
{
	int	status;

	status = 1;
	status = pipe_syntax_error(head);
	if (!status)
		return (0);
	status = redir_syntax_error(head);
	if (!status)
		return (0);
	status = quote_syntax_error(head);
	if (!status)
		return (0);
	return (status);
}

void	expand_all_word_tokens(t_list *token_list, t_env *env_vars)
{
	t_list	*cur;

	cur = token_list;
	while (cur)
	{
		if (cur->token->type == T_WORD)
			process_token_expansion(cur->token, env_vars);
		cur = cur->next;
	}
}

int	init(t_list **head, t_env *env_vars)
{
	char	*input;
	int		status;

	status = 1;
	rl_on_new_line();
	input = readline("minishell$ ");
	if (!input)
	{
		rl_clear_history();
		write(1, "exit\n", 5);
		exiting(g_exit_status);
	}
	if (*input)
		add_history(input);
	tokenize(input, head);
	status = syntax_error(*head);
	if (!status)
	{
		free(input);
		*head = NULL;
		return (0);
	}
	expand_all_word_tokens(*head, env_vars);
	free(input);
	return (status);
}
