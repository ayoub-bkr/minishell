/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukent <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:40:36 by aboukent          #+#    #+#             */
/*   Updated: 2025/08/30 15:40:37 by aboukent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_heredoc(t_token_type type, int set)
{
	static bool	val;

	if (set == 1)
		val = (type == T_HEREDC);
	return (val);
}

bool	should_expand_heredoc(int set, bool value, int i)
{
	static bool	val[16];

	if (set)
		val[i] = value;
	if (set == -1)
	{
		i = 0;
		while (i < 16)
		{
			val[i] = true;
			i++;
		}
	}
	return (val[i]);
}
