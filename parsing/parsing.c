#include "../minishell.h"

void	tokenize(char *str, t_list **head)
{
	while (str && *str)
	{
		while (*str == ' ' || *str == '\t')
			str++;
		if (tokenize_word(&str, head))
			str = str;
		else if (tokenize_metachar(&str, head))
			str = str;
	}
}

int	count_w_tokens(t_list *head)
{
	t_list	*cur;
	int		i;

	if (!head)
		return (0);
	cur = head;
	i = 0;
	while (cur)
	{
		if (cur->token->type == T_WORD)
			i++;
		cur = cur->next;
	}
	return (i);
}

static void	init_command(t_command *cmd, int len)
{
	int	i;

	i = 0;
	cmd->args = gc_calloc(sizeof(char *) * (len + 1));
	cmd->args[len] = NULL;
	while (i <= len)
	{
		cmd->args[i] = NULL;
		i++;
	}
	cmd->redir = NULL;
	cmd->pipe_out = 0;
	cmd->next = NULL;
}

static void	fill_command_args_and_redirs(t_list **cur, t_command *cmd, int *j)
{
	while (*cur)
	{
		if ((*cur)->token->type == T_PIPE)
			break ;
		else if ((*cur)->token->type >= T_RED_IN
			&& (*cur)->token->type <= T_HEREDC)
			handle_redirection(cur, cmd, (*cur)->token->type);
		else
			cmd->args[(*j)++] = ft_strdup((*cur)->token->str);
		*cur = (*cur)->next;
	}
}

void	parsing(t_command **command, t_list *head)
{
	t_list		*cur;
	t_command	*cmd;
	int			len;
	int			j;

	if (!head)
		return ;
	cur = head;
	cmd = gc_calloc(sizeof(t_command));
	len = count_w_tokens(head);
	j = 0;
	init_command(cmd, len);
	fill_command_args_and_redirs(&cur, cmd, &j);
	cmd_lstaddback(command, cmd);
	if (cur && cur->token->type == T_PIPE && cur->next)
		parsing(command, cur->next);
}
