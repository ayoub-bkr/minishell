/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_syntax_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 02:51:38 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 02:51:39 by mohel-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

/*
int	handle_syntax_error(char *str, int *i, char *in_quote)
{
	while (str[*i])
	{
		if ((str[*i] == '"' || str[*i] == '\'') && !*in_quote)
			*in_quote = str[*i];
		else if (str[*i] == *in_quote)
			*in_quote = 0;
		(*i)++;
	}
	if (*in_quote)
	{
		printf("[+] syntax error\n");
		printf("{-} unclosed quote\n");
		return (0);
	}
	return (1);
}

int	quote_syntax_error(t_list *head)
{
	t_list	*cur;
	char	*str;
	int		i;
	char	in_quote;

	if (!head)
		return (0);
	cur = head;
	while (cur)
	{
		if (cur->token->type == T_WORD)
		{
			str = cur->token->str;
			i = 0;
			in_quote = 0;
			if (!handle_syntax_error(str, &i, &in_quote))
				return (0);
		}
		cur = cur->next;
	}
	return (1);
}
*/

int quote_syntax_error(t_list *head)
{
    if (!head)
        return (0);
    
    t_list *cur = head;
    while (cur)
    {
        if (cur->token->type == T_WORD)
        {
            char *str = cur->token->str;
            int i = 0;
            char in_quote = 0;  // 0=none, '"'=double, '\''=single
            
            while (str[i])
            {
                if ((str[i] == '"' || str[i] == '\'') && !in_quote)
                    in_quote = str[i];  // Start quote
                else if (str[i] == in_quote)
                    in_quote = 0;       // End quote
                i++;
            }
            
            if (in_quote)  // Quote was opened but never closed
            {
                printf("[+] syntax error\n");
                printf("{-} unclosed quote\n");
                return (0);
            }
        }
        cur = cur->next;
    }
    return (1);
}
