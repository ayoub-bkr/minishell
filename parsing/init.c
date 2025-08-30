/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:40:41 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/30 15:40:43 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	redir_error_print(t_list *cur, t_token_type pttype, t_token_type cttype)
{
	if (cttype == T_RED_OUT || cttype == T_RED_IN || cttype == T_APPEND
		|| cttype == T_HEREDC)
	{
		if (pttype == T_RED_OUT || pttype == T_RED_IN || pttype == T_APPEND
			|| pttype == T_HEREDC)
		{
			g_exit_status = 2;
			ft_putstr_fd("[+] syntax error\n{-} redirections", 1, 2);
			return (0);
		}
		if (!cur->next || cur->next->token->type != T_WORD)
		{
			g_exit_status = 2;
			ft_putstr_fd("[+] syntax error\n{-} redirections", 1, 2);
			return (0);
		}
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
		if (!redir_error_print(cur, pttype, cttype))
			return (0);
		pttype = cur->token->type;
		cur = cur->next;
	}
	return (1);
}

void	expand_all_word_tokens_2(t_list *cur, t_list **token_list, t_list *prev)
{
	if (!has_quote(0, 0) && cur->token->str && !cur->token->str[0]
		&& cur->token->type == T_WORD)
	{
		if (cur == *token_list)
			(*token_list) = (*token_list)->next;
		else if (prev)
			prev->next = cur->next;
	}
}

void	expand_all_word_tokens(t_list **token_list, t_env *env_vars)
{
	t_list	*cur;
	t_list	*prev;
	int		i;

	i = 0;
	cur = *token_list;
	prev = NULL;
	should_expand_heredoc(-1, 0, 0);
	while (cur)
	{
		if (prev)
			is_heredoc(prev->token->type, 1);
		if (cur->token->type == T_WORD)
			process_token_expansion(cur->token, env_vars);
		expand_all_word_tokens_2(cur, token_list, prev);
		if (is_heredoc(0, 0) && has_quote(0, 0))
		{
			should_expand_heredoc(1, false, i);
			i++;
		}
		prev = cur;
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
	expand_all_word_tokens(head, env_vars);
	free(input);
	return (status);
}
