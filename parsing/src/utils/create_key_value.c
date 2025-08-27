/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_key_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 03:37:49 by mohel-mo          #+#    #+#             */
/*   Updated: 2025/08/27 03:37:50 by mohel-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	create_key_value(t_env *single_var)
{
	single_var->key = get_key(single_var->var);
	single_var->value = get_value(single_var->var);
}

void	create_key_value_pairs(t_env *env_vars)
{
	t_env	*cur;

	cur = env_vars;
	while (cur)
	{
		create_key_value(cur);
		cur = cur->next;
	}
}

char	*get_key(char *s)
{
	int		i;
	char	*tmp;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	tmp = gc_calloc(i + 1);
	if (!tmp)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		tmp[i] = s[i];
		i++;
	}
	tmp[i] = 0;
	return (tmp);
}

char	*get_value(char *s)
{
	int		i;
	int		start;
	int		len;
	char	*tmp;

	i = 0;
	start = 0;
	len = 0;
	while (s[i] && s[i] != '=')
		i++;
	start = ++i;
	while (s[len + start])
		len++;
	tmp = gc_calloc(len + 1);
	if (!tmp)
		return (NULL);
	i = 0;
	while (s[start])
		tmp[i++] = s[start++];
	tmp[i] = 0;
	return (tmp);
}
