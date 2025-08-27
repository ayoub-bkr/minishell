/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:21:50 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 02:21:52 by mohel-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    t_redir *r = gc_calloc(sizeof(t_redir));
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
		if (cur->token->type == T_WORD)
			i++;
		cur = cur->next;
	}
	return (i);
}

void parsing(t_command **command, t_list *head)
{
	if (!head)
		return ;

	t_list *cur = head;
    t_command *cmd = gc_calloc(sizeof(t_command));
	int len = count_w_tokens(head);
	int j = 0;
	int i = 0;
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

    while (cur)
    {
		if (cur->token->type == T_PIPE)
		{
			break ;
		}
		else if (cur->token->type == T_RED_IN)
		{
			printf("we got here\n");
			if (cur->next)
				cur = cur->next;
			if (!cur->token->str)
				printf("this is waq\n");
			redir_addback(&cmd->redir, redir_new(ft_strdup("<"), 0));
		}
		else if (cur->token->type == T_RED_OUT)
		{
			cur = cur->next;
			redir_addback(&cmd->redir, redir_new(ft_strdup(cur->token->str), 1));
		}
		else if (cur->token->type == T_APPEND)
		{
			cur = cur->next;
			redir_addback(&cmd->redir, redir_new(ft_strdup(cur->token->str), 2));
		}
		else if (cur->token->type == T_HEREDOC)
		{
			cur = cur->next;
			redir_addback(&cmd->redir, redir_new(ft_strdup(cur->token->str), 3));
		}
		else
			cmd->args[j++] = ft_strdup(cur->token->str);
		cur = cur->next;
    }
	cmd_lstaddback(command, cmd);
	if (cur && cur->token->type == T_PIPE && cur->next)
			parsing(command, cur->next);
    return ;
}
