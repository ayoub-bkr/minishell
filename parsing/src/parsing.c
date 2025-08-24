#include "../../minishell.h"

void	cmd_lstaddback(t_command **head, t_command *new)
{
	t_command	*tmp;

	if (!new)
		return ;
	new->pipe_out = 0;
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		tmp->pipe_out = 1;
	}
}

t_redir *redir_new(char *file, int type)
{
    t_redir *r = malloc(sizeof(t_redir));
    if (!r)
        return NULL;
    r->file = file;
    r->type = type;
    r->next = NULL;
    return r;
}

void redir_addback(t_redir **head, t_redir *new)
{
    t_redir *tmp;
    if (!*head)
        *head = new;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

void tokenize(char *str, t_list **head)
{
	while (str && *str)
	{
		while (*str == ' ' || *str == '\t') str++;
		if (tokenize_word(&str, head))
		{ }
	  	else if (tokenize_metachar(&str, head))
		{ }
	}
}

int count_w_tokens(t_list *head)
{
	t_list	*cur;
	int		i;

	if (!head)
		return (0);
	cur = head;
	i = 0;
	while (cur)
	{
		cur = cur->next;
		i++;
	}
	return (i);
}

t_command *parsing(t_command **command, t_list *head)
{
	// NOTE: that cur->token->str we pass to redir_new is actually null 
	// when we print it! i think i should replace it
	if (!head)
		return (NULL);

	t_list *cur = head;
    t_command *cmd = malloc(sizeof(t_command));
	int len = count_w_tokens(head);
	int j = 0;
    cmd->args = malloc(sizeof(char *) * (len + 1));
	cmd->args[len] = NULL;
    cmd->redir = NULL;
    cmd->pipe_out = 0;
    cmd->next = NULL;

    while (cur)
    {
		if (cur->token->type == T_PIPE)
		{
			break ;
		}
		else if (cur->token->type == T_RED_IN)
		{
			cur = cur->next;
			redir_addback(&cmd->redir, redir_new(cur->token->str, 0));
		}
		else if (cur->token->type == T_RED_OUT)
		{
			cur = cur->next;
			redir_addback(&cmd->redir, redir_new(cur->token->str, 1));
		}
		else if (cur->token->type == T_APPEND)
		{
			cur = cur->next;
			redir_addback(&cmd->redir, redir_new(cur->token->str, 2));
		}
		else if (cur->token->type == T_HEREDOC)
		{
			cur = cur->next;
			redir_addback(&cmd->redir, redir_new(cur->token->str, 3));
		}
		else
			cmd->args[j++] = cur->token->str;
		cur = cur->next;
    }
	cmd_lstaddback(command, cmd);
	if (cur && cur->token->type == T_PIPE && cur->next)
			parsing(command, cur->next);
    return cmd;
}

/* new part
t_command *parsing(char *cmd_str)
{
    t_command *cmd = malloc(sizeof(t_command));
    int i = 0, j = 0;
    char **tokens = ft_split(cmd_str, ' '); // split by space
    cmd->args = malloc(sizeof(char *) * (count_tokens(tokens) + 1));
    cmd->redir = NULL;
    cmd->pipe_out = 0;
    cmd->next = NULL;

    while (tokens[i])
    {
        if (strcmp(tokens[i], "<") == 0 && tokens[i+1])
            redir_addback(&cmd->redir, redir_new(ft_strdup(tokens[++i]), 0));
        else if (strcmp(tokens[i], ">") == 0 && tokens[i+1])
            redir_addback(&cmd->redir, redir_new(ft_strdup(tokens[++i]), 1));
        else if (strcmp(tokens[i], ">>") == 0 && tokens[i+1])
            redir_addback(&cmd->redir, redir_new(ft_strdup(tokens[++i]), 2));
        else if (strcmp(tokens[i], "<<") == 0 && tokens[i+1])
            redir_addback(&cmd->redir, redir_new(ft_strdup(tokens[++i]), 3));
        else
            cmd->args[j++] = ft_strdup(tokens[i]);
        i++;
    }
    cmd->args[j] = NULL;
    return cmd;
}
*/
