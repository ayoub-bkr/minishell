/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:41:05 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/30 15:41:06 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pipe_syntax_error2(t_list *cur, int i, t_token_type prev_token_type)
{
	if (cur->token->type == T_PIPE)
	{
		if (i == 0)
		{
			g_exit_status = 2;
			ft_putstr_fd("[+] syntax error\n{-} Invalid start", 1, 2);
			return (0);
		}
		if (prev_token_type == T_PIPE)
		{
			g_exit_status = 2;
			ft_putstr_fd("[+] syntax error\n{-} pipes Error", 1, 2);
			return (0);
		}
		if (!cur->next || cur->next->token->type != T_WORD)
		{
			g_exit_status = 2;
			ft_putstr_fd("[+] syntax error\n{-} invalid command", 1, 2);
			return (0);
		}
	}
	return (-1);
}

int	pipe_syntax_error(t_list *head)
{
	t_list			*cur;
	int				i;
	t_token_type	prev_token_type;
	int				result;

	if (!head)
		return (0);
	cur = head;
	i = 0;
	while (cur)
	{
		result = pipe_syntax_error2(cur, i, prev_token_type);
		if (result != -1)
			return (result);
		prev_token_type = cur->token->type;
		i++;
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
