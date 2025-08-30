/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_syntax_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:41:39 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/30 15:41:40 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	check_quotes_in_str(char *str)
{
	int		i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && !in_quote)
			in_quote = str[i];
		else if (str[i] == in_quote)
			in_quote = 0;
		i++;
	}
	if (in_quote)
	{
		ft_putstr_fd("[+] syntax error\n{-} unclosed quote", 1, 2);
		return (0);
	}
	return (1);
}

int	process_word_token(t_list *cur)
{
	if (cur->token->type == T_WORD)
		return (check_quotes_in_str(cur->token->str));
	return (1);
}

int	quote_syntax_error(t_list *head)
{
	t_list	*cur;

	if (!head)
		return (0);
	cur = head;
	while (cur)
	{
		if (!process_word_token(cur))
			return (0);
		cur = cur->next;
	}
	return (1);
}
