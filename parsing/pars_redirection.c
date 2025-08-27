/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:27:54 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 18:27:55 by mohel-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

t_redir	*redir_new(char *file, int type)
{
	t_redir	*r;

	r = gc_calloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->file = file;
	r->type = type;
	r->next = NULL;
	return (r);
}

void	redir_addback(t_redir **head, t_redir *new)
{
	t_redir	*tmp;

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

void	handle_redirection(t_list **cur, t_command *cmd, int type)
{
	if ((*cur)->next)
		*cur = (*cur)->next;
	if (type == T_RED_IN)
		redir_addback(&cmd->redir, redir_new(ft_strdup("<"), 0));
	else if (type == T_RED_OUT)
		redir_addback(&cmd->redir, redir_new(ft_strdup((*cur)->token->str), 1));
	else if (type == T_APPEND)
		redir_addback(&cmd->redir, redir_new(ft_strdup((*cur)->token->str), 2));
	else if (type == T_HEREDC)
		redir_addback(&cmd->redir, redir_new(ft_strdup((*cur)->token->str), 3));
}
